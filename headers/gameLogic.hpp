// Various methods used for physics game logic
#ifndef _GAMELOGIC_HPP_
#define _GAMELOGIC_HPP_
#include <vector>
#include <GLM/glm.hpp>
#include "asteroid.hpp"
class agent;

#define AGENT_OBJ_I 0
#define BOUNDS_OBJ_I 1
#define ASTEROID_START 2
#define ASTEROID_COUNT 100
#define BOUNDS_R 50
#define MAX_DISTANCE_TARGET 100

namespace gameLogic {
    // Type of states
    enum STATE {
        FAIL,   // Hit a bound or asteroid
        ARENA,  // Still in the game
        EXIT,   // Reach the goal state
    };

    // Available recognized exclusive actions availble to the agent
    // Future considerations, combination of actions (such as +x&+y)
    // As simulation becomes more involved may require new/removing actions
    enum ACTION {
        POS_Z, // Activate thrusters in the +z direction
        POS_X, // Activate thrusters in the +x direction
        POS_Y, // Activate thrusters in the +y direction
        NEG_Z, // Activate thrusters in the -z direction
        NEG_X, // Activate thrusters in the -x direction
        NEG_Y, // Activate thrusters in the -y direction
        HOLD   // Do not activate any thrusters
    };

    // Get d^2 between two 3D points (in km) in m^2
    double getSquaredDistance(const glm::vec3 a, const glm::vec3 b);

    // Get d between two 3D points (in km) in m
    double getDistance(const glm::vec3 a, const glm::vec3 b);

    // the angle between two points and origin in XY plane
    double getThetaXY(glm::vec3 p1, glm::vec3 p2);

    // the angle between two points and origin in XZ plane
    double getThetaXZ(glm::vec3 p1, glm::vec3 p2);

    // Normalize a vector (magnitude of 1)
    glm::vec3 normalizeVector(glm::vec3 v);

    // Derive a 3D component vector describing the acceleration on the ship agent by the asteroids
    glm::vec3 netAccelerationShip(const agent * ship, const std::vector<Asteroid>& asteroids);

    // Determine game state by where the ship is    
    STATE shipLocationState(const agent * ship, const std::vector<Asteroid>& asteroids, double bounds_r);
}

#endif
