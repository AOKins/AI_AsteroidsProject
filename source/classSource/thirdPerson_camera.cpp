#ifndef _THIRDPERSONCAMERA_CPP_
#define _THIRDPERSONCAMERA_CPP_

#include <STB/stb.h>
#include "thirdPerson_camera.hpp"
#include "../functions/transformDerive.cpp"

// Constructor
thirdPerson_camera::thirdPerson_camera(object_gl * target) : camera() {
    this->target = target;
    this->position.x = 1;
    this->heading = M_PI /2.0f;
}

void thirdPerson_camera::updateView() {
    this->up = this->abs_up;// * getRotationX(this->pitch);
    glm::vec3 direction = this->position.x * this->abs_foward * myRotationMethods::getRotationZ(this->pitch) * myRotationMethods::getRotationY(this->heading);
    // Update view matrix (projection + translation)
    this->view = glm::lookAt(this->target->getPosition() + direction, this->target->getPosition(), this->up);
    
    // Setting projection to have projection contents of view matrix, but ommitting translation elements
    this->projection = glm::mat4(
        glm::vec4(view[0]),
        glm::vec4(view[1]),
        glm::vec4(view[2]),
        glm::vec4(0,0,0,1)
    );
    // Update perspective matrix
    this->perspective = glm::perspective(this->fov, this->aspect, this->clip_near, this->clip_far);
}

#endif
