#ifndef _SKYBOX_HPP_
#define _SKYBOX_HPP_

#include "shader.hpp"

// Simple skybox class to manage its rendering responsibilities
// Elements of this approach based on Scott Griffith's examples
class skyBox {
private:
    GLuint vertexArray_id, vertexBuffer_id, elementBuffer_id, texture_ID;

    bool textured;

    // The points of a simple 1x1x1 cube, constant base values
    const float h_dim = 0.5f;
    const glm::vec4 points[36] = {
        // Front Side
            // Top-Right
        glm::vec4(-h_dim,  h_dim,  h_dim, 1.0),
        glm::vec4( h_dim,  h_dim,  h_dim, 1.0),
        glm::vec4( h_dim, -h_dim,  h_dim, 1.0),
            // Bottom-Left
        glm::vec4( h_dim, -h_dim,  h_dim, 1.0),
        glm::vec4(-h_dim, -h_dim,  h_dim, 1.0),
        glm::vec4(-h_dim,  h_dim,  h_dim, 1.0),
        // Left Side
            // Top-Right
        glm::vec4(-h_dim,  h_dim,  h_dim, 1.0),
        glm::vec4(-h_dim,  h_dim, -h_dim, 1.0),
        glm::vec4(-h_dim, -h_dim, -h_dim, 1.0),
            // Bottom-Left
        glm::vec4(-h_dim, -h_dim, -h_dim, 1.0),
        glm::vec4(-h_dim,  h_dim,  h_dim, 1.0),
        glm::vec4(-h_dim, -h_dim,  h_dim, 1.0),
        
        // Back Side
            // Bottom-Left
        glm::vec4(-h_dim,  h_dim, -h_dim, 1.0),
        glm::vec4(-h_dim, -h_dim, -h_dim, 1.0),
        glm::vec4( h_dim, -h_dim, -h_dim, 1.0),
            // Top-Right
        glm::vec4( h_dim, -h_dim, -h_dim, 1.0),
        glm::vec4( h_dim,  h_dim, -h_dim, 1.0),
        glm::vec4(-h_dim,  h_dim, -h_dim, 1.0),

        // Right Side
            // Top-Right
        glm::vec4( h_dim,  h_dim, -h_dim, 1.0),
        glm::vec4( h_dim, -h_dim, -h_dim, 1.0),
        glm::vec4( h_dim,  h_dim,  h_dim, 1.0),
            // Bottom-Left
        glm::vec4( h_dim,  h_dim,  h_dim, 1.0),
        glm::vec4( h_dim, -h_dim,  h_dim, 1.0),
        glm::vec4( h_dim, -h_dim, -h_dim, 1.0),
        
        // Top Side
            // Top-Right
        glm::vec4( h_dim,  h_dim,  h_dim, 1.0),
        glm::vec4( h_dim,  h_dim, -h_dim, 1.0),
        glm::vec4(-h_dim,  h_dim, -h_dim, 1.0),
            // Bottom-Left
        glm::vec4( h_dim,  h_dim,  h_dim, 1.0),
        glm::vec4(-h_dim,  h_dim, -h_dim, 1.0),
        glm::vec4(-h_dim,  h_dim,  h_dim, 1.0),

        // Bottom Side
            // Top-Right
        glm::vec4(-h_dim, -h_dim, -h_dim, 1.0),
        glm::vec4(-h_dim, -h_dim,  h_dim, 1.0),
        glm::vec4( h_dim, -h_dim, -h_dim, 1.0),
            // Bottom-Left
        glm::vec4( h_dim, -h_dim,  h_dim, 1.0),
        glm::vec4(-h_dim, -h_dim,  h_dim, 1.0),
        glm::vec4( h_dim, -h_dim, -h_dim, 1.0)
    };

public:
    // Shader for the SkyBox object only
    shader * skyBoxShader;
    // Cosntructor that takes in file paths for vertex and fragment shaders, along with optional directory to textures
    // For textureDir, put in file path to appriopriate directory, assumes the explicit file names as sc_[side].bmp
    skyBox(const char* vertexPath, const char* fragmentPath, const char* textureDir = "");
    
    // Destructor for skyBox
    ~skyBox();

    // Method for loading in the textures
    void loadTextures(const char * directory);
    // Method for loading in a given side texture textures
    void loadSideTexture(GLint texture_ID, GLenum side, const char * filePath);

    // Method for handling the rendering
    void renderSkyBox(glm::mat4 perspective = glm::mat4(), glm::mat4 projection = glm::mat4());
};

#endif
