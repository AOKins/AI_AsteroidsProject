#ifndef _THIRDPERSONCAMERA_HPP_
#define _THIRDPERSONCAMERA_HPP_

#include "camera.hpp"
#include "object_gl.hpp"

class thirdPerson_camera : public camera {
protected:
    object_gl * target;
    // Private methods used realtimeUpdate
    void updateView();

public:
    // Constructor for 3rd Person camera requires a target object to follow
    thirdPerson_camera(object_gl * target);
};

#endif
