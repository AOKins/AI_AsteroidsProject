#ifndef _GRAPHICS_METHODS_CPP_
#define _GRAPHICS_METHODS_CPP_

#include "shader.hpp"
#include "object_gl.hpp"
#include "stbi_include.h"
#include "../functions/transformDerive.cpp"

// Namespace for general common openGL methods that I will be needing
namespace GLmethods {

// Method for rendering a general object with provided shader
// Input: objShader - pointer to shader for rendering
//              obj - pointer to object to be rendered
// verticiesBuff_ID - ID for vertex buffer of the object
//        uvBuff_ID - ID for UV values for the object
// Output: The object should be drawn using the provided shader
void render_object(shader * objShader, object_gl * obj, GLuint verticiesBuff_ID) {
    GLuint vertexID = objShader->getLocation("position");

    // Setting the translation transform for the object
    //glm::mat4 translation(1.0f);
    //translation = glm::translate(translation, obj->getPosition());

    //objShader->setMat4("translation", translation);
    objShader->setMat4("toSpace", obj->getToSpace());
    // Creating the scale matrix to appriopriately set the size of the object
    glm::mat4 scaleMatrix = glm::mat4x4(obj->getScale());
    scaleMatrix[3].w = 1.0f; // Correcting the w componenet

    // Setting scale matrix into shader
    //objShader->setMat4("scale", scaleMatrix);
    //// Setting the orientation of the object (rotating to correctly according to it's bank, heading, and pitch)
    //objShader->setMat4("ori", obj->getRotation());

    objShader->setVec3("obj_color", obj->color);

    // Linking vertex buffer
    glEnableVertexAttribArray(vertexID); //Recall the vertex ID
    glBindBuffer(GL_ARRAY_BUFFER, verticiesBuff_ID);//Link object buffer to vertex_ID
    glVertexAttribPointer( // Index into the buffer
            vertexID,  // Attribute in question
            4,         // Number of elements per vertex call (vec4)
            GL_FLOAT,  // Type of element
            GL_FALSE,  // Normalize? Nope
            0,         // No stride (steps between indexes)
            0);        // initial offset

    // Call to draw the tirangle, starting at index 0 and number of vertices to render for triangles (using size of entire array divided by size of each vertex)
    glDrawArrays(GL_TRIANGLES, 0, obj->verticies.size());
}

// Method for loading an object into the device
// Input: obj - pointer to object to be loaded in
// Output: object added to buffer and ID values set
void load_object(shader * objShader, object_gl * obj,
                 GLuint  * elementBuff_ID, GLuint * vertexArray_ID, GLuint * verticiesBuff_ID
                 ) {
    glUseProgram(objShader->shaderID);

    // Creating vertex array
    glCreateVertexArrays(1, vertexArray_ID);

    glCreateBuffers(1, verticiesBuff_ID);
    glCreateBuffers(1, elementBuff_ID);

    glBindVertexArray(*vertexArray_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *elementBuff_ID);
    glBindBuffer(GL_ARRAY_BUFFER, *verticiesBuff_ID);

    GLuint vertexID = objShader->getLocation("position");

    // Send triangle data to the buffer, specifing that it is to the array buffer, providing size and address,
    // followed by the usage (which here is as static drawing)
    glBufferData(GL_ARRAY_BUFFER, obj->verticies.size() * sizeof(obj->verticies[0]),
                 obj->verticies.data(), GL_STATIC_DRAW);
    
    // Setting attributes to the vertex array so that it knows how to uses the vertex array
    // resource that was used in this usage and acquirng this current state of understanding is 
    // https://learnopengl.com/Getting-started/Hello-Triangle
    // first argument setting input variable being attributed which is zero as the location for the verticies
    //      data is set to 0 in the shader (location = 0 for position)
    // second argument specifies number of values (the vertex data is comprised of 4 data points as well as color)
    // third argument gives size of each value (floats)
    // fourth sets to normalize the data if true
    // fifth argument gives the distance between each set of data
    // sixth gives offset in the buffer to start off with (which is 0 as there is no need for offsetting) 
    glVertexAttribPointer(vertexID, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    
    // Enabling the arrays that have been created to be used in the vertex shader
    glEnableVertexAttribArray(*vertexArray_ID);
    
}

// Simple method for handling the deletion of the vertex arrays
void delete_arrays(std::vector<GLuint> &vertexArrays) {
    glDeleteVertexArrays(vertexArrays.size(), vertexArrays.data());
}

};

#endif
