#ifndef _ASTEROID_CPP_
#define _ASTEROID_CPP_

#include "asteroid.hpp"
#include <SDL2/SDL_stdinc.h> // M_PI

// Constructor
Asteroid::Asteroid(double r, double density, object_gl * obj) {
        this->radius = 1000*r;
        // Density is in  g/cm^3, converting to kg/m^3
        this->mass = 1000*density*M_PI*this->radius*this->radius*this->radius*4/3;
        this->obj_ref = obj;
}

// Getter for mass
const double Asteroid::getMass() const{
    return this->mass;
}

// Getter for position
const glm::vec3 Asteroid::getPosition() const {
    return this->obj_ref->getPosition();
}

// Getter for radius
const double Asteroid::getRadius() const {
    return this->radius;
}

#endif
