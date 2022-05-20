#ifndef _AGENT_HPP_
#define _AGENT_HPP_

#include "actor.hpp"
#include "gameLogic.hpp"
class rep_state;
#include <string>

// The entity responsible for controlling an actor (base agent is human controlled)
class agent {
protected:
    // The actor this agent controls
    actor * host_ref;
public:
    // Constructor
    agent(object_gl * host);

    // Destructor
    ~agent();

    // Getters
    double getMass() const;    
    glm::vec3 getPosition() const;
    glm::vec3 getVelocity() const;
    object_gl * getObjRef() const;
    const actor * getActorRef() const;
    double getCollisionRadius() const;
    double getUsedDelta() const;

    // Setters
    void setPosition(glm::vec3 new_pos);
    void setVelocity(glm::vec3 setV);
    void setObject(object_gl * ref);

    // Should only be called when setting up a new scenario
    void resetDelta();

    // Determine action to take
    // deltaTime is in seconds
    virtual gameLogic::ACTION action(double deltaTime);

    // (AI Only) return string of weight values for user info
    virtual std::string currentWeightsString();

    // Methods primarily used by the AI agent variant, observe the transition from one state to another
    // Input: prevState         - state prior before now,
    //          action          - what the agent did to get to current,
    //          currentState    - the state we are in,
    //              deltaTime   - how much time elapsed between states
    virtual void observe(rep_state * prevState, gameLogic::ACTION action, rep_state * currentState, double deltaTime);
    
    // Get V value (AI only)
    // Input: state - state to explore q values for
    //  deltaTime   - how much time expected to elapse (needed for Q value consideration)
    virtual const double getVval(rep_state * state, double deltaTime) const;
};

#endif
