#ifndef _AI_AGENT_HPP_
#define _AI_AGENT_HPP_

#include <vector>
#include <functional>   // For std::function
#include <random>       // For std::mt19937_64
#include "object_gl.hpp"
#include "rep_state.hpp"

#define DEF_EPSILON 0.5
#define DEF_TIMEDELAY 30
#define DEF_ALPHA 0.25
#define DEF_GAMMA 0.8

using namespace gameLogic;

class ai_agent : public agent {
protected:
    std::vector<double> weights;
    std::vector<std::function<double(rep_state*,ACTION, double)>> features;

    double epsilon; // Odds for policy instead of random choice in trainingMode
    double alpha; // Learning rate
    double gamma; // discount rate
    bool trainingMode;
    double decisionEvalStepSize;
    double timeSinceLastDecision;
    ACTION currentAction;
    game_state * curr_state;
    rep_state * lastStateCheck; // Represents last state when a decision was being made

    std::mt19937_64 rng; // Using this as a persistant and better distributed rng machine to drive decision making

    // Get the Q value for a given state and action
    double getQValue(ACTION action, rep_state * state, double deltaTime) const;

    // Go through each action for q values, returning the action with best Q value
    ACTION determineActionFromQValue(double deltaTime) const;

    // Return true if going with weights+features, false if random
    // Uses epsilon
    bool goWithPolicy();

    // Update the weights from the transition to a new state
    void update(rep_state * currentState, ACTION action, rep_state * resultingState, double deltaTime, double reward);
public:
    // Constructor
    // Input: host - object to make actor out of
    //      gameStatePtr - get a reference to the state info
    //      decisionEvalStepSize - how much simulation time (in seconds) before making another decision
    //      e - epsilon, % chance during learning it will go with policy
    //      a - alpha, learning rate
    //      y - gamma, discount rate
    ai_agent(object_gl * host, game_state * gameStatePtr, double decisionEvalStepSize = DEF_TIMEDELAY, double e = DEF_EPSILON, double a = DEF_ALPHA, double y = DEF_GAMMA);

    // Destructor, just needs to clear lastStateCheck property
    ~ai_agent();

    // Toggle wether to learn with random decision making
    void toggleTrainingMode();

    // Output the weights as a string prefixed with "Weights"
    std::string currentWeightsString();
    
    // Set the game state the agent references from
    void setState(game_state * newState);

    // Determine action to take, given time elapsed
    // Returns the action decided upon
    ACTION action(double deltaTime);
    
    // Observe to call as wrapper towards the update of weights
    void observe(rep_state * current, gameLogic::ACTION action, rep_state * next_state, double deltaTime);

    // Get the V value of a state
    double getVvalue(rep_state * state, double deltaTime);
};

#endif
