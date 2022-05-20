#ifndef _ACTOR_HPP_
#define _ACTOR_HPP_
#include <GLM/glm.hpp>
#include "object_gl.hpp"
#include <iostream>

// Wrapper for an "actor" in the game (different from agent which "controls the actor")
// Currently hardcoded to represent "the Enterprise 1701-D" as our actor
class actor {
    // Reference to graphics object it's gonna interact with
    object_gl * obj_ref;

    glm::vec3 velocity;
    double mass;
    double radius;
    double deltaUsed;
    
public:
    // Constructor
    actor(object_gl * ref);
    // Getters
    glm::vec3 getPosition();
    glm::vec3 getVelocity();
    double getMass();
    double getCollisionRadius();
    object_gl * getObjRef();
    double getUsedDelta();

    // Setters
    void setDelta(double delta);
    void setObjRef(object_gl * newRef);
    void setPosition(glm::vec3 pos);
    void setVelocity(glm::vec3 v);
    
    // Get what the resulting acceleration would be
    const double projDelta(int dim, double delta) const ;

    // Apply thrust with given direction and duration
    void deltaThrust(int dim, double delta);

    // Perform enough thrust to stop thrust (pretty much never used now)
    void zeroThrust(int dim);
    
    // Apply a vector describing acceleration to velocity given time duration that acceleration occurs
    void applyAcceleration(glm::vec3 a, double deltaTime);
};

#endif
