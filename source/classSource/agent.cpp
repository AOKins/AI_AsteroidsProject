#ifndef _AGENT_CPP_
#define _AGENT_CPP_

#include "agent.hpp"
#include "gameLogic.hpp"
#include <SDL2/SDL.h>
using namespace gameLogic;

// Constructor
agent::agent(object_gl * host) {
    this->host_ref = new actor(host);   
}

// Destructor
agent::~agent() {
    delete this->host_ref;
}

// Getters //

double agent::getCollisionRadius() const {
    return this->host_ref->getCollisionRadius();
}
double agent::getMass() const {
    return this->host_ref->getMass();
}
glm::vec3 agent::getPosition() const {
    return this->host_ref->getPosition();
}
glm::vec3 agent::getVelocity() const {
    return this->host_ref->getVelocity();
}
object_gl * agent::getObjRef() const {
    return this->host_ref->getObjRef();
}
const double agent::getVval(rep_state * state, double deltaTime) const {
    return 0;
}
const actor * agent::getActorRef() const {
    return this->host_ref;
}
double agent::getUsedDelta() const {
    return this->host_ref->getUsedDelta();
}

// End of Getters //

void agent::observe(rep_state * prevState, gameLogic::ACTION action, rep_state * currentState, double deltaTime) {}

// Setters //
void agent::setPosition(glm::vec3 new_pos) {
    return this->host_ref->setPosition(new_pos);
}
void agent::setVelocity(glm::vec3 setV) {
    this->host_ref->setVelocity(setV);
}
void agent::setObject(object_gl * ref) {
    this->host_ref->setObjRef(ref);
}

// End of Setters //

// Should only be called when setting up a new scenario
void agent::resetDelta() {
    this->host_ref->setDelta(0);
}

std::string agent::currentWeightsString() {
    return "No weights for human agent!";
}

// Determine action to take
// deltaTime is in seconds
// For base agent, read keyboard input (human controlled)
ACTION agent::action(double deltaTime) {
    const Uint8 * keyboardState = SDL_GetKeyboardState(NULL);
    
    // Forcing only one thruster at a time by using else if instead of just ifs
    if (keyboardState[SDL_SCANCODE_W] == SDL_PRESSED)     { this->host_ref->deltaThrust(2,deltaTime);  return POS_Z; }
    else if (keyboardState[SDL_SCANCODE_S] == SDL_PRESSED)     { this->host_ref->deltaThrust(2,-deltaTime); return NEG_Z;}
    else if (keyboardState[SDL_SCANCODE_A] == SDL_PRESSED)     { this->host_ref->deltaThrust(0,deltaTime);  return POS_X;}
    else if (keyboardState[SDL_SCANCODE_D] == SDL_PRESSED)     { this->host_ref->deltaThrust(0,-deltaTime); return NEG_X;}
    else if (keyboardState[SDL_SCANCODE_SPACE] == SDL_PRESSED) { this->host_ref->deltaThrust(1,deltaTime);  return POS_Y;}
    else if (keyboardState[SDL_SCANCODE_LCTRL] == SDL_PRESSED) { this->host_ref->deltaThrust(1,-deltaTime); return NEG_Y;}
    else return HOLD;
}

#endif
