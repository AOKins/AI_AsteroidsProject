#ifndef _GAMESTATE_CPP_
#define _GAMESTATE_CPP_

#include "game_state.hpp"
#include "rep_state.hpp"
#include "../functions/gameLogic.cpp"
#include <SDL2/SDL.h>
using namespace gameLogic;

// Constructor
game_state::game_state(agent * a, unsigned int seed, object_gl * objArray) {
    srand(seed);
    this->userAgent = a;
    this->deltaTime = 0;
        // Setup asteroids
    this->asteroidInfo.clear();
    float scale;

    for (int i = ASTEROID_START; i <= ASTEROID_COUNT + ASTEROID_START; i++) {
        scale = float(rand() % 10) / 2.5f; // scale is in km
        objArray[i].setScale(scale);
        glm::vec3 p = glm::vec3(float(rand() % BOUNDS_R*10 - BOUNDS_R*5) / 10.0f,float(rand() % BOUNDS_R*10 - BOUNDS_R*5) / 10.0f, 3*scale-float(rand() % MAX_DISTANCE_TARGET*10) / 10.f);
        objArray[i].setPosition(p);
        objArray[i].color = glm::vec3(0.f,2.0f,0.00f);

        this->asteroidInfo.push_back(Asteroid(scale,2,&objArray[i]));
    }
    // Setup Ship
    this->userAgent->getObjRef()->setScale(0.641f);
    this->userAgent->getObjRef()->setPosition(glm::vec3(0,0,-MAX_DISTANCE_TARGET));
    this->userAgent->setVelocity(glm::vec3(0,0,0));
    this->userAgent->resetDelta();

    this->userAgent->getObjRef()->color = glm::vec3(0.2f,0.2f,1.0f);

    // Setup bounds graphiucal representation
    this->boundsRepresentation = &objArray[BOUNDS_OBJ_I];
    this->boundsRepresentation->color = glm::vec3(1,0,0);
    this->boundsRepresentation->setStretch(glm::vec3(BOUNDS_R,MAX_DISTANCE_TARGET,BOUNDS_R));
    this->boundsRepresentation->setPitch(M_PI/2.0f);
    this->boundsRepresentation->setHeading(M_PI/2.0f);
    this->boundsRepresentation->setPosition(glm::vec3(0,0,-MAX_DISTANCE_TARGET));
    this->state_type = ARENA;
}

// Get the state that it's in
STATE game_state::getStateType() const {
    return this->state_type;
}
// Get the agent's position
glm::vec3 game_state::getUserAgentPos() const {
    return this->userAgent->getPosition();
}
// Get the agent's velocity
glm::vec3 game_state::getUserAgentVel() const {
    return this->userAgent->getVelocity();
}
// Get the agent's radius size
const double game_state::getUserAgentCollisionRadius() const {
    return this->userAgent->getCollisionRadius();
}
// Get the asteroid data
const std::vector<Asteroid> & game_state::getRefAsteroidData() const {
    return this->asteroidInfo;
}
// Get the delta spent by the agent so far
double game_state::getUsedDelta() const {
    return this->userAgent->getUsedDelta();
}
// Get reference to the actor being controlled
const actor * game_state::getActorRef() const {
    return this->userAgent != nullptr ? this->userAgent->getActorRef() : nullptr;
}
// Get reference to the agent
const agent * game_state::getAgentRef() const {
    return this->userAgent;
}

// Get project upcoming velocity (does not change the velocity of the agent)
// Considers gravity effect as well
glm::vec3 game_state::updatedShipVelocity(double deltaTime) const {
    glm::vec3 acceleration = gameLogic::netAccelerationShip(this->userAgent, this->asteroidInfo);
    glm::vec3 velocity = this->userAgent->getVelocity();
    velocity.x += acceleration.x*deltaTime;
    velocity.y += acceleration.y*deltaTime;
    velocity.z += acceleration.z*deltaTime;
    return velocity;
}

// Get project upcoming position (does not change the position of the agent)
glm::vec3 game_state::updatedShipLocation(double deltaTime) const {
    glm::vec3 pos = this->userAgent->getPosition();
    glm::vec3 velocity = this->userAgent->getVelocity();
    pos.x += velocity.x*deltaTime;
    pos.y += velocity.y*deltaTime;
    pos.z += velocity.z*deltaTime;
    return pos;
}

// Process time elapsed
// Input: deltaOccured - how much time to step next, in seconds
//          stepCount - how many counts within to process
    // Return: ARENA if all good, if ran into FAIL or EXIT will stop premptively and return the resulting state
STATE game_state::tick(double deltaStep, int stepCount) {
    double stepSize = deltaStep/stepCount;
    for (int i = 0; i < stepCount && this->state_type == ARENA; i++) {
        rep_state startState = rep_state(this);
        this->deltaTime += stepSize;

        
        ACTION actionTaken = this->userAgent->action(stepSize);
        
        this->userAgent->setVelocity(this->updatedShipVelocity(stepSize));
        this->userAgent->setPosition(this->updatedShipLocation(stepSize));

        STATE s = gameLogic::shipLocationState(this->userAgent, this->asteroidInfo,BOUNDS_R);
        this->state_type = s;
        rep_state nowState = rep_state(this);
        
        this->userAgent->observe(&startState,actionTaken, &nowState,deltaTime);
    }
    return this->state_type;
}

// Determine the "Score" based on current state values
double game_state::determineScore() const {
    STATE s = gameLogic::shipLocationState(this->userAgent, this->asteroidInfo,BOUNDS_R*1000);
    double score;

    switch (s) {
        case ARENA:
            score = 1e2 + this->getUserAgentPos().z - 10*this->getUsedDelta();
            break;
        case EXIT:
            score = 1e4 + this->getUserAgentPos().z - 1e3*this->getUsedDelta();
            break;
        case FAIL:
        default:
            score = this->getUserAgentPos().z + this->getUserAgentVel().z/1e2 - abs(this->getUserAgentVel().x) - abs(this->getUserAgentVel().y) - 100*this->getUsedDelta();
    }
    return score;
}

#endif
