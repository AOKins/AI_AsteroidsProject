#ifndef _GAMESTATE_HPP_
#define _GAMESTATE_HPP_


#include "agent.hpp"
#include "asteroid.hpp"
#include "object_gl.hpp"
#include "gameLogic.hpp"
using namespace gameLogic;

class game_state {
protected:
    // The state type this is in right now
    STATE state_type;
    // Reference to the user agent in the game
    agent * userAgent;
    // List of asteroid data
    std::vector<Asteroid> asteroidInfo;
    // Reference to the object data that represents the cylindrical bounds
    object_gl * boundsRepresentation;
    // What the bound radius is
    double gameBoundsRadius;
    // How much time has elapsed
    double deltaTime;
public:
    // Constructor
    // seed is for setupScenario
    // objArray is expected to have list of asteroids
    game_state(agent * a, unsigned int seed, object_gl * objArray);

    // Get the state that it's in
    STATE getStateType() const;

    // Get the agent's position
    glm::vec3 getUserAgentPos() const;

    // Get the agent's velocity
    glm::vec3 getUserAgentVel() const;

    // Get the agent's radius size
    const double getUserAgentCollisionRadius() const;

    // Get the asteroid data
    const std::vector<Asteroid> & getRefAsteroidData() const;

    // Get the delta spent by the agent so far
    double getUsedDelta() const;
    // Get reference to the actor being controlled
    const actor * getActorRef() const;
    // Get reference to the agent
    const agent * getAgentRef() const;

    // Get project upcoming position (does not change the position of the agent)
    // Considers gravity effect as well
    glm::vec3 updatedShipLocation(double deltaTime) const;
    // Get project upcoming velocity (does not change the velocity of the agent)
    glm::vec3 updatedShipVelocity(double deltaTime) const;
    
    // Process time elapsed
    // Input: deltaOccured - how much time to step next, in seconds
    //          stepCount - how many counts within to process
    // Return: ARENA if all good, if ran into FAIL or EXIT will stop premptively and return the resulting state
    STATE tick(double deltaOccured, int stepCount);

    // Determine the "Score" based on current state values
    double determineScore() const;
};

#endif
