#ifndef _OBJECTGL_HPP_
#define _OBJECTGL_HPP_
// Note for AI Project: Lots of stuff (such as parentObj) are irrelevant but remain as legacy code

#include <string>
#include <vector>
#include "shader.hpp"

#include <GLM/glm.hpp>
#include <GL/gl.h>

// Structure to hold properties of a render object
// Contents based on Scott's example of object loading
class object_gl {
    float bank, heading, pitch; // Orientation of the object in radians
    glm::vec3 position;         // Position in 3D space
    float scale;                // Scaler for the object
    glm::vec3 stretchScale;     // Stretch the object (shearing) along coordinates

    bool textured;

    // Various useful matrices 
    glm::mat4 translation;
    glm::mat4 rotation;
    glm::mat4 toSpace; // Combination of rotation and translation and scaler, takes the object from object space to it's larger space
    
    void updateMatrices();

    // Property to connecting this object to a parent object for more complex objects
    object_gl * parentObj; // Get the parent's toSpace to perform hierarchical translation
    glm::vec3 hierTranslate; // Position of the object relative to the parent

public:
    glm::vec3 color;
    // Publicly accessible vertices, terxture vertices, and normals
    std::vector<glm::vec4> verticies;

    // Constuctor that takes in string to where the .obj file is located and optional texture image file
    // Defaults the orientation to 0,0,0 as well as position
    object_gl(std::string objPath);
    // Method used in constructor to properly load contents of .obj file
    void load_from_file(std::string filePath);
    
    // Getters //
    glm::vec3 getPosition();
    float getBank();
    float getHeading();
    float getPitch();
    float getScale();
    glm::mat4 getObjectTranslation();
    glm::mat4 getHierarchyTranslation();
    glm::mat4 getRotation();
    glm::mat4 getToSpace();
    // Setters //
    void setBank(float new_bank);
    void setHeading(float new_heading);
    void setPitch(float new_pitch);
    void setPosition(glm::vec3 new_pos);
    void setScale(float new_scale);
    void setStretch(glm::vec3 new_stretch);
    // Setter for parent object with option argument for translation
    void setParent(object_gl * new_parent, glm::vec3 setHierTranslate = glm::vec3(0,0,0));
};

#endif
