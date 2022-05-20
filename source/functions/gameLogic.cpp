#ifndef _GAMELOGIC_CPP_
#define _GAMELOGIC_CPP_
#include "agent.hpp"
#include "asteroid.hpp"
#include "gameLogic.hpp"
#include <SDL2/SDL_stdinc.h>

#define G 6.674e-11

namespace gameLogic {

    // Take in a and b (locations in km)
    // Return the squared distance in m
    double getSquaredDistance(const glm::vec3 a, const glm::vec3 b) {
        glm::vec3 t = a-b;
        return 1e6*(t.x*t.x+t.y*t.y+t.z*t.z);
    }
    
    // Get d between two 3D points
    double getDistance(const glm::vec3 a, const glm::vec3 b) {
        return std::sqrt(getSquaredDistance(a,b));
    }
    // the angle between two points and origin in XZ plane
    double getThetaXY(glm::vec3 p1, glm::vec3 p2) {
        return abs(p1.x-p2.x) <= 0.001 ? (M_PI/2.0f-0.001) : 
            atan( abs(p1.y-p2.y) / abs(p1.x-p2.x)) ;
    }
    // the angle between two points and origin in XZ plane
    double getThetaXZ(glm::vec3 p1, glm::vec3 p2) {
        double xy1 = p1.x+p1.y;
        double xy2 = p2.x+p2.y;
        
        return abs(xy1-xy2) <= 0.001 ? (M_PI/2.0f-0.001) : 
            atan( abs(p1.z-p2.z) / abs(xy1-xy2) );
    }

    // Normalize a vector (magnitude of 1)
    glm::vec3 normalizeVector(glm::vec3 v) {
        double m = sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
        return glm::vec3(v.x/m,v.y/m,v.z/m);
    }

    // Derive a 3D component vector describing the acceleration on the ship agent by the asteroids
    glm::vec3 netAccelerationShip(const agent * ship, const std::vector<Asteroid>& asteroids) {
        // Not actually force, but acceleration
        glm::vec3 net_f = glm::vec3(0,0,0);
        glm::vec3 f_vector, net_offset;
        double d, f_magnitude, thetaXY, thetaXZ;

        for (Asteroid a : asteroids) {
            f_vector = glm::vec3(0,0,0);
            d = getSquaredDistance(a.getPosition(), ship->getPosition());
            f_magnitude = G*a.getMass()/(d);
            
            net_offset = normalizeVector(a.getPosition() - ship->getPosition());
            
            f_vector.x += net_offset.x*f_magnitude;
            f_vector.y += net_offset.y*f_magnitude;
            f_vector.z += net_offset.z*f_magnitude;
            
            net_f += f_vector;
        }
        return net_f;
    }

    // Determine game state by where the ship is
    STATE shipLocationState(const agent * ship, const std::vector<Asteroid>& asteroids, double bounds_r) {
        glm::vec3 pos = ship->getPosition();
        if (pos.x*pos.x+pos.y*pos.y > bounds_r*bounds_r || pos.z < -(MAX_DISTANCE_TARGET+2)) return FAIL;
        if (pos.z > 0) return EXIT;
        
        double d;
        for (Asteroid a : asteroids) {
            
            d = getSquaredDistance(a.getPosition(), ship->getPosition());
            double minAllowed = ship->getCollisionRadius()+a.getRadius();
            if (d < (minAllowed*minAllowed)) {
                return FAIL;
            }
        }
        return ARENA;
    }
};

#endif
