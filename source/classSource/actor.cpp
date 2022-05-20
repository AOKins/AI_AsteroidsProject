#ifndef _ACTOR_CPP_
#define _ACTOR_CPP_

#include "actor.hpp"

// Constructor
actor::actor(object_gl * ref) {
    this->obj_ref = ref;
    this->velocity = glm::vec3(0,0,0);
    this->mass = 5e9; // Mass of the enterpise
    this->radius = 641/2; // Enterprise is 641m diameter at most
    this->deltaUsed = 0;
}

// Getters //

glm::vec3 actor::getPosition() {
    return this->obj_ref->getPosition();
}
glm::vec3 actor::getVelocity() {
    return this->velocity;
}
double actor::getMass() {
    return this->mass;
}
double actor::getCollisionRadius() {
    return this->radius;
}
object_gl* actor::getObjRef() {
    return this->obj_ref;
}
double actor::getUsedDelta() {
    return this->deltaUsed;
}

// End of Getters //
// Setters //
void actor::setPosition(glm::vec3 pos) {
    this->obj_ref->setPosition(pos);
}

void actor::setDelta(double delta) {
    this->deltaUsed = delta;
}

void actor::setObjRef(object_gl * newRef) {
    this->obj_ref = newRef;
}

void actor::setVelocity(glm::vec3 v) {
    this->velocity = v;
}
// End of Setters //

// Get what the resulting acceleration would be
const double actor::projDelta(int dim, double delta) const {
    return delta*0.001f;
}

// Apply thrust with given direction and duration
void actor::deltaThrust(int dim, double delta) {
    double thrustAmount = delta*0.001f;
    this->velocity[dim] += thrustAmount;
    this->deltaUsed += thrustAmount;
}

// Perform enough thrust to stop thrust (pretty much never used now)
void actor::zeroThrust(int dim) {
    this->deltaUsed += this->velocity[dim];
    this->velocity[dim] = 0;
}

// Apply a vector describing acceleration to velocity given time duration that acceleration occurs
void actor::applyAcceleration(glm::vec3 a, double deltaTime) {
    this->velocity.x += a.x*deltaTime;
    this->velocity.y += a.y*deltaTime;
    this->velocity.z += a.z*deltaTime;
}

#endif
