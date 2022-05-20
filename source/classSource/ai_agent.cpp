#ifndef _AIAGENT_CPP_
#define _AIAGENT_CPP_

#include "ai_agent.hpp"
#include <algorithm>
#include "../functions/features.cpp"
using namespace gameLogic;

// Constructor
// Input: host - object to make actor out of
//      gameStatePtr - get a reference to the state info
//      decisionEvalStepSize - how much simulation time (in minutes) before making another decision
//      e - epsilon, % chance during learning it will go with policy
//      a - alpha, learning rate
//      y - gamma, discount rate
ai_agent::ai_agent(object_gl * host, game_state * gameStatePtr, double timeStep, double e, double a, double y) : agent(host) {
    this->curr_state = gameStatePtr;
    this->epsilon = e;
    this->alpha = a;
    this->gamma = y;
    this->decisionEvalStepSize = timeStep;
    this->timeSinceLastDecision = 0;
    this->lastStateCheck = new rep_state();

    this->rng = std::mt19937_64(time(NULL));
    // Setting what the fatures are and initial weights
    this->features.push_back(agentFeatures::ZVelocity);
    this->features.push_back(agentFeatures::XVelocity);
    this->features.push_back(agentFeatures::YVelocity);
    this->features.push_back(agentFeatures::DistanceFromZ);
    this->features.push_back(agentFeatures::XposMag);
    this->features.push_back(agentFeatures::YposMag);
    this->features.push_back(agentFeatures::DesiredAction);
    this->features.push_back(agentFeatures::UnDesiredAction);
    this->features.push_back(agentFeatures::PropAsteroidsPassed);
    this->features.push_back(agentFeatures::DistFromCloseSurf);
    this->features.push_back(agentFeatures::DistanceFromBounds);

    this->weights = std::vector<double>(this->features.size(), 1.00);
    this->trainingMode = true;
}

// Destructor, just needs to clear lastStateCheck property
ai_agent::~ai_agent() {
    delete this->lastStateCheck;
}

// Get the Q value for a given state and action
double ai_agent::getQValue(ACTION action, rep_state * state, double deltaTime) const {
    double total_q = 0;
    rep_state projState(state->getPresentState(), this, action, deltaTime);
    // Assuming weights and features are same size
    for (int i = 0; i < features.size(); i++) {
        total_q += this->weights[i]*this->features[i](&projState, action, deltaTime);
    }
    return total_q;
}

// Determine action to take, given time elapsed
// Returns the action decided upon
ACTION ai_agent::action(double deltaTime) {
    this->timeSinceLastDecision += deltaTime;

    if (this->timeSinceLastDecision >= this->decisionEvalStepSize) { 
        // Determine action either by policy or randomization       
        this->currentAction = this->goWithPolicy() ? determineActionFromQValue(this->timeSinceLastDecision) : ACTION(this->rng() % (HOLD+1));
        // Update the last state a decision was made with
        delete this->lastStateCheck;
        this->lastStateCheck = new rep_state(this->curr_state);
    }
    // Translate enum value to call to functions
    switch (this->currentAction) {
        case(POS_X):
            this->host_ref->deltaThrust(0,deltaTime);
            break;
        case(NEG_X):
            this->host_ref->deltaThrust(0,-deltaTime);
            break;
        case(POS_Y):
            this->host_ref->deltaThrust(1,deltaTime);
            break;
        case(NEG_Y):
            this->host_ref->deltaThrust(1,-deltaTime);
            break;
        case(POS_Z):
            this->host_ref->deltaThrust(2,deltaTime);
            break;
        case(NEG_Z):
            this->host_ref->deltaThrust(2,-deltaTime);
            break;
        case(HOLD):
            break;
    }
    return this->currentAction;
}

// Return true if going with weights+features, false if random
// Uses epsilon
bool ai_agent::goWithPolicy() {
    return (this->trainingMode == false) || ((this->rng() % 100) < 100*this->epsilon);
}

// Set the game state the agent references from
void ai_agent::setState(game_state * new_state) {
    this->curr_state = new_state;
    this->timeSinceLastDecision = 0;
}

// Go through each action for q values, returning the action with best Q value
ACTION ai_agent::determineActionFromQValue(double deltaTime) const {
    ACTION bestAction = HOLD;
    rep_state repCurr = rep_state(this->curr_state);
    double currV = this->getQValue(bestAction,&repCurr,deltaTime);
    
    for (int a = 0; a < (int)HOLD; a++) {
        ACTION act = ACTION(a);
        double newQ = this->getQValue(act,&repCurr,deltaTime);
        if (newQ > currV) {
            currV = newQ;
            bestAction = act;
        }
    }
    return bestAction;
}

// Toggle wether to learn with random decision making
void ai_agent::toggleTrainingMode() {
    this->trainingMode = !this->trainingMode;
}

// Get the V value of a state
double ai_agent::getVvalue(rep_state * state, double deltaTime) {
    if (state->getStateType() != ARENA) {
        return state->getDetermineScore();
    }
    ACTION bestAction = HOLD;
    double currV = this->getQValue(HOLD,state,deltaTime);

    for (int a = 0; a < (int)HOLD; a++) {
        ACTION act = ACTION(a);
        double newQ = this->getQValue(act,state,deltaTime);

        if (newQ > currV) {
            currV = newQ;
            bestAction = act;
        }
    }
    return currV;
}

// Observe to call as wrapper towards the update of weights
void ai_agent::observe(rep_state * prevState, gameLogic::ACTION action, rep_state * currentState, double deltaTime) {
    if (this->timeSinceLastDecision >= this->decisionEvalStepSize) {
        double reward = 0;
        if (currentState->getStateType() == gameLogic::ARENA) {
            reward = currentState->getDetermineScore() - prevState->getDetermineScore() + 0.5;
        }
        this->update(prevState, action, currentState, this->timeSinceLastDecision, reward);
        this->timeSinceLastDecision = 0;
    }
}

// Output the weights as a string prefixed with "Weights"
std::string ai_agent::currentWeightsString() {
    std::string report = "Weights:";
    for (int i = 0 ; i < this->weights.size(); i++) {
        report += std::to_string(this->weights[i]) + " ";
    }
    return report;
}

// Update the weights from the transition to a new state
void ai_agent::update(rep_state * lastState, ACTION action, rep_state * resultingState, double deltaTime, double reward) {
    double expected = this->getQValue(action, lastState, deltaTime);
    double result = this->gamma * this->getVvalue(resultingState, deltaTime);
    double difference = reward + result - expected;
    // Update each weight
    for (int i = 0; i < this->features.size(); i++) {
        this->weights[i] = this->weights[i] + this->alpha * difference * this->features[i](lastState,action, deltaTime);
    }
    std::cout << this->currentWeightsString() << std::endl;
}

#endif