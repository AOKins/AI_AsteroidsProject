#ifndef _OBJECT_CPP_
#define _OBJECT_CPP_
#include "object_gl.hpp"
#include "../functions/pop_substr.cpp"
#include "../functions/transformDerive.cpp"
#include "stbi_include.h"

#include <fstream>

// Constructor with given file path to the .obj file
object_gl::object_gl(std::string objPath) {
    this->position = glm::vec3();
    this->bank = 0;
    this->heading = 0;
    this->pitch = 0;
    this->position = glm::vec3(0,0,0);
    this->stretchScale = glm::vec3(1,1,1);
    this->textured = false;
    // Default scale is 1.0
    this->scale = 1.0f;
    // Default parent object as NULL
    this->parentObj = NULL;
    // Load from file for other data
    load_from_file(objPath);
    updateMatrices();
}

// Method used in constructor to properly load contents of .obj file (implementation largely (98%) from Scott Griffith's example)
// Input is string to where the file is
// Output is verticies,uvs, and normals vectors in struct are set according to file contents
void object_gl::load_from_file(std::string filePath) {
    //File to load in
    std::ifstream in(filePath, std::ios::in);

    //Check to make sure file opened
    if (!in) {
        char buf[50];
        sprintf(buf, "OBJ file not found!");
        exit(EXIT_FAILURE);
    }

    //Temp vectors to hold data
    //These will need to be indexed into the output vectors based on face info
    std::vector<glm::vec4> tempVert; // from verticies lines 'v <x> <y> <z>'
    //This might need to be a more complicated structure, we can get away with strict indexing
    std::vector<GLuint> tempFace; // from face line 'f <v1>/<t1>/<n1> <v2>/<t2>/<n2> <v3>/<t3>/<n3>', should be indexes
    std::vector<glm::vec2> tempUVs; // from texture line 'vt <x> <y>'
    std::vector<glm::vec4> tempNorm; // from a normal line 'vn <x> <y> <z>' //Should this be a vec3 or vec4?

    std::string line;       // Complete line pulled from file    
    std::string sub = "";   // Substring working space
    GLfloat num;            // Temp floats from line processing
    
    while (std::getline(in, line)) { //grab every line of the file
            //Depending on what is in the line, parse differently     
            if (line.substr(0,2) == "v ") {
                //Process vertex line
                sub = line.substr(2); //Isolate substring without 'v'
                //vector to be filled
                glm::vec4 tVec(0.0f,0.0f,0.0f,1.0f);
                //Expect 3 numbers, iterate three times
                //Isolate number and clip substring [x,y,z,1]
                tVec[0] = std::stof(pop_substr(sub," "));
                tVec[1] = std::stof(pop_substr(sub," "));
                tVec[2] = std::stof(pop_substr(sub," "));
                //Push vec4 onto verticies vector
                tempVert.push_back(tVec);
            } else if (line.substr(0,2) == "f ") {
                //Process face line 
                // Faces line f 14/25/9 60/19/9 56/97/9 : f <vertex1>/<texture1>/<normal1> <vertex2>/<texture2>/<normal2> <vertex3>/<texture3>/<normal3>
                sub = line.substr(2); //Current sub string of line
                //Expect 3 number sets, loop three times
                for(int i = 0; i < 3; i++) {
                    //Isolate next triangle set
                    //First before / /
                    tempFace.push_back( std::stoi(pop_substr(sub,"/")) ); //sub_part 0 == vertex index
                    //second value in triplet (between the / and /)
                    tempFace.push_back( std::stoi(pop_substr(sub,"/")) ); //sub_part 1 == texture coordinate index
                    //third value in triplet (after both /, " " up next)
                    tempFace.push_back( std::stoi(pop_substr(sub," ")) ); //sub_part 2 == normal index                    
                }
                //Already pushed info onto faces
            } else if (line.substr(0,3) == "vt ") {
                //Process Texture mappings
                sub = line.substr(3); //Isolate substring without 'vt '
                glm::vec2 tUV(0.0f,0.0f);
                //Expect 2 numbers, loop twice
                tUV[0] = std::stof(pop_substr(sub," ")); //x
                tUV[1] = std::stof(pop_substr(sub," ")); //y
                //Push vec2 onto texture vector
                tempUVs.push_back(tUV);                
            } else if (line.substr(0,3) == "vn ") {
                //Process normal line
                sub = line.substr(3); //Isolate substring without 'vt '
                //4D? is w:0?
                glm::vec4 tNorm(0.0f,0.0f,0.0f,0.0f);
                //Expect 3 numbers, loop thrice
                //Isolate number and clip substring
                tNorm[0] = std::stof(pop_substr(sub," "));
                tNorm[1] = std::stof(pop_substr(sub," "));
                tNorm[2] = std::stof(pop_substr(sub," "));
                //Push normal onto tempNormal vector
                tempNorm.push_back(tNorm);
            } else {
                //other kind of line, ignoring
            }
        } //end of line parsing
    //Clear out output vectors (just to be safe)
    this->verticies.clear();
    //At this point out temp vectors are full of data
    // tempVert, tempUVs and tempNorm are indexed (starting at 0) in file order
    // tempFace correlates everything together in sets of 9 values (three triplets)
    // Using the data in tempFace the output vectors will be filled with the correct (in order) data
    //                   0    1    2    3    4    5    6    7    8
    // Faces striping: <v1>/<t1>/<n1> <v2>/<t2>/<n2> <v3>/<t3>/<n3>
    //Because the data in tempFace is striped buy sets of three triplets, step forward by 9 each time
    for(int i = 0; i < tempFace.size(); i += 9 ){
        //Pull data into verticies
        //                                   VVV Index offset pattern
        //                          VVV Holds vertex index to pull from tempVery (offset from starting at 1 to 0)    
        //                 VVV Indexed vertex info
        this->verticies.push_back(tempVert[tempFace[i+0]-1]); //v1
        this->verticies.push_back(tempVert[tempFace[i+3]-1]); //v2
        this->verticies.push_back(tempVert[tempFace[i+6]-1]); //v3
    }
}

// Getter for bank angle
float object_gl::getBank() {
    return this->bank;
}

// Getter for heading angle
float object_gl::getHeading() {
    return this->heading;
}

// Getter for pitch angle
float object_gl::getPitch() {
    return this->pitch;
}

// Getter for the current position
glm::vec3 object_gl::getPosition() {
    return this->position;
}

// Getter for the scale
float object_gl::getScale() {
    return this->scale;
}

// Getter for the translation matrix for this object
glm::mat4 object_gl::getObjectTranslation() {
    return this->translation;
}

// Getter for the translation matrix for this object
glm::mat4 object_gl::getHierarchyTranslation() {
    return glm::mat4(
        glm::vec4(1.0, 0.0, 0.0, 0.0),
        glm::vec4(0.0, 1.0, 0.0, 0.0),
        glm::vec4(0.0, 0.0, 1.0, 0.0),
        glm::vec4(this->hierTranslate,1.0));
}

// Getter for the rotation matrix for this object
glm::mat4 object_gl::getRotation() {
    return this->rotation;
}

// Getter for the getToWorld matrix for this object
glm::mat4 object_gl::getToSpace() {
    if (this->parentObj != NULL) {
        return this->parentObj->getToSpace() * 
               this->getHierarchyTranslation() * 
               this->toSpace;
    }
    else {
        return this->toSpace;
    }
}

// Method that should called whenever changing position or orientation values for the object 
void object_gl::updateMatrices() {
    // Creating the scale matrix to appriopriately set the size of the object
    glm::mat4 scaleMatrix = glm::mat4x4(this->getScale());
    
    scaleMatrix[0].x *= this->stretchScale.x;
    scaleMatrix[1].y *= this->stretchScale.y;
    scaleMatrix[2].z *= this->stretchScale.z;
    scaleMatrix[3].w = 1.0f; // Correcting the w componenet



    // Derive orientation
    this->rotation = myRotationMethods::getRotationMatrix(this->bank, this->heading, this->pitch); //
    this->translation = glm::mat4(
        glm::vec4(1.0, 0.0, 0.0, 0.0),
        glm::vec4(0.0, 1.0, 0.0, 0.0),
        glm::vec4(0.0, 0.0, 1.0, 0.0),
        glm::vec4(this->position,1.0f));
    // Derive toWorld matrix using orientation and translation
    this->toSpace = this->translation * this->rotation * scaleMatrix;
}

// Setter for bank
void object_gl::setBank(float new_bank) {
    this->bank = new_bank;
    updateMatrices();
}

// Setter for heading
void object_gl::setHeading(float new_heading) {
    this->heading = new_heading;
    updateMatrices();
}

// Setter for pitch
void object_gl::setPitch(float new_pitch) {
    this->pitch = new_pitch;
    updateMatrices();
}

// Setter for scale
void object_gl::setScale(float new_scale) {
    this->scale = new_scale;
    updateMatrices();
}

// Setter for position
void object_gl::setPosition(glm::vec3 new_pos) {
    this->position = new_pos;
    updateMatrices();
}

// Setter for parent object pointer and translation to where this object is in relation to the parent
void object_gl::setParent(object_gl * new_parent, glm::vec3 setHierTranslate) {
    this->parentObj = new_parent;
    this->hierTranslate = setHierTranslate;
}

void object_gl::setStretch(glm::vec3 new_stretch) {
    this->stretchScale = new_stretch;
    this->updateMatrices();
}

#endif
