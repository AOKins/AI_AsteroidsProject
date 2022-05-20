#ifndef _ASTEROID_HPP_
#define _ASTEROID_HPP_

#include <GLM/glm.hpp>
#include "object_gl.hpp"

class Asteroid {
    double mass; // in kg
    double radius; // in m
    object_gl * obj_ref; // Reference to graphic representation (gets position from it)
public:
    // Constructor
    Asteroid(double r, double density, object_gl * obj);

    // Getter for mass
    const double getMass() const;

    // Getter for position
    const glm::vec3 getPosition() const;

    // Getter for radius
    const double getRadius() const;
};

#endif
