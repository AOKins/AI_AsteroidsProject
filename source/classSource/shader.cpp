#ifndef _SHADER_CPP_
#define _SHADER_CPP_
#include "shader.hpp"
#include <fstream>
#ifdef DEBUG_BUILD
    #include <iostream>
#endif
#include <type_traits>

const char* vs_source = "#version 450 core\nlayout (location = 0) in vec4 position;\nuniform mat4 translation;uniform mat4 scale;uniform mat4 ori;uniform mat4 toSpace;uniform mat4 camera;uniform mat4 perspective;\nuniform vec3 obj_color;\nout vec4 fragColor;\nvoid main() {\ngl_Position = (perspective * camera * toSpace * position) / position.w;float v = gl_Position.z/100.0f;fragColor = vec4(obj_color.x - v, obj_color.y - v, obj_color.z - v, 1.0f);}";
//    #include "../../shaders/vertex.shader";
const char* fs_source = "#version 450 core\nout vec4 color;\nin vec4 fragColor;\nvoid main(void) {color = fragColor;}";
//    #include "../../shaders/fragment.shader";

// Constructor
shader::shader() {
    int success;
    char errorLog[512];
    GLuint vertexID, fragmentID;

    // Creating and compiling vertex shader
    // Setting to create new vertex shader, returning id of the shader object
    vertexID = glCreateShader(GL_VERTEX_SHADER);

    const GLchar * vertex_source = vs_source;
    
    // Sending the source code for the vertex shader
    glShaderSource(vertexID, 1, &vertex_source, NULL);

    // Compiling the source code that has been sent
    glCompileShader(vertexID);
    glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
        #ifdef DEBUG_BUILD
    if (!success) {
        glGetShaderInfoLog(vertexID, 512, NULL, errorLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILURE\n" << errorLog;
    }
        #endif

    // Creating and compiling fragment shader
    fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Sending the source code for the fragment shader
    glShaderSource(fragmentID, 1, &fs_source, NULL);

    // Compiling the source code that has been sent
    glCompileShader(fragmentID);

    glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
        #ifdef DEBUG_BUILD
    if (!success) {
        glGetShaderInfoLog(vertexID, 512, NULL, errorLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILURE\n" << errorLog;
    }
        #endif

    // Creating shader program, attaching vertex and fragment shaders to be then finally linked
    this->shaderID = glCreateProgram();
    glAttachShader(this->shaderID, vertexID);
    glAttachShader(this->shaderID, fragmentID);
    glLinkProgram(this->shaderID);
    // Now that shader program has been setup, can delete shader objects to clean up unused resources
    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);
}

// Constructor with given file paths to vertex and fragment source code files
shader::shader(const char * vertexName, const char * fragmentName) {
    int success;
    char errorLog[512];
    GLuint vertexID, fragmentID;

    // Creating and compiling vertex shader
    // Setting to create new vertex shader, returning id of the shader object
    vertexID = glCreateShader(GL_VERTEX_SHADER);

    const GLchar * vertex_source = load_from_file(vertexName);
    
    // Sending the source code for the vertex shader
    glShaderSource(vertexID, 1, &vertex_source, NULL);

    // Compiling the source code that has been sent
    glCompileShader(vertexID);
    glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
        #ifdef DEBUG_BUILD
    if (!success) {
        glGetShaderInfoLog(vertexID, 512, NULL, errorLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILURE\n" << errorLog;
    }
        #endif

    // Creating and compiling fragment shader
    fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar * frag_source = load_from_file(fragmentName);
    
    // Sending the source code for the fragment shader
    glShaderSource(fragmentID, 1, &frag_source, NULL);

    // Compiling the source code that has been sent
    glCompileShader(fragmentID);

    glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
        #ifdef DEBUG_BUILD
    if (!success) {
        glGetShaderInfoLog(vertexID, 512, NULL, errorLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILURE\n" << errorLog;
    }
        #endif

    // Creating shader program, attaching vertex and fragment shaders to be then finally linked
    this->shaderID = glCreateProgram();
    glAttachShader(this->shaderID, vertexID);
    glAttachShader(this->shaderID, fragmentID);
    glLinkProgram(this->shaderID);
    // Now that shader program has been setup, can delete shader objects to clean up unused resources
    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);
}

// Really simple load_from_file method to read file contents with given path
// Input: filePath - char array that gives file path to file (ASSUMED CORRECT WITH NO CHECK)
// Output: returns const char array of file contents
const char* shader::load_from_file(const char * filePath) {
    char * fileContent;
    FILE * file;
    size_t size;
    file = fopen(filePath, "rb");

    if (!file) return 0;

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file,0,SEEK_SET);    
    fileContent = new char [size + 1];

    if (!fileContent) return 0;

    fread(fileContent, 1, size, file);
    fileContent[size] = 0;
    fclose(file);

    return fileContent;
}

shader::~shader() {
    glDeleteProgram(shaderID);
}

// Implementing the set methods for uniform variables
// Based on example from learnopengl.com 
// Bool uniform
void shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(shaderID, name.c_str()), (int)value); 
}

// Integer uniform
void shader::setInt(const std::string &name, int value) const { 
    glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value); 
}

// Float uniform
void shader::setFloat(const std::string &name, float value) const { 
    glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value); 
}

// Float array uniform
void shader::setNfloat(const std::string &name, int n, float &start) const { 
    glUniform1fv(glGetUniformLocation(shaderID, name.c_str()), n, &start); 
}

// Vec3 array uniform
void shader::setNvec3(const std::string &name, int n, glm::vec3 &start) const {
    glUniform3fv(glGetUniformLocation(shaderID, name.c_str()), n, glm::value_ptr(start)); 
}

// Vec4 array uniform
void shader::setNvec4(const std::string &name, int n, glm::vec4 &start) const {
    glUniform4fv(glGetUniformLocation(shaderID, name.c_str()), n, glm::value_ptr(start)); 
}

// Vec3 uniform
void shader::setVec3(const std::string &name, glm::vec3 value) const {
    glUniform3fv(glGetUniformLocation(shaderID, name.c_str()), 1, glm::value_ptr(value)); 
}

// Vec4 uniform
void shader::setVec4(const std::string &name, glm::vec4 value) const {
    glUniform4fv(glGetUniformLocation(shaderID, name.c_str()), 1, glm::value_ptr(value)); 
}

// Template array uniform
template<typename T>
void shader::setNvalue(const std::string &name, int n, T &start) const {
    glUniform1fv(glGetUniformLocation(shaderID, name.c_str()),n, start);
}

// 4x4 matrix uniform
void shader::setMat4(const std::string &name, glm::mat4 matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

// Getter for attribute location of given name
GLuint shader::getLocation(const std::string &name) {
    return glGetAttribLocation(this->shaderID, name.c_str());
}

#endif
