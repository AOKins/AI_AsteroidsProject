#ifndef _MYAPP_H_
#define _MYAPP_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <vector>

#include "object_gl.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "thirdPerson_camera.hpp"
#include "skyBox.hpp"
#include "agent.hpp"
#include "asteroid.hpp"
#include "ai_agent.hpp"
#include "game_state.hpp"

// Main application class that handles running the application
class application {
protected: 
    bool running;
    int fps; // Desired framerate to run at
    bool wireFrameMode;
    unsigned int lineWidth; // Wire frame mode rendering line width
        // How many steps to do within a tick
            // If tick has 10ms delta and step is 2, will perform 2 5ms actionable steps
    unsigned int aiStepCount;
        // How many ticks to perform in a frame
            // Affectively the multiplier of the speed of the simulation, 1x is ~real, 2x is twice, etc.
    unsigned int simGraphicsStepSize;
    
    bool renderSkybox; // If true, render a space background
    SDL_Window * window;
    SDL_GLContext context;

    // Shader applications - objectsShader for rendering the objects themselves, shadowMapShader for generating shadow map to determine lighting
    shader * objectsShader;

    // Dimensions of the window
    int window_width, window_height;

    // Vector that holds object data
    std::vector<object_gl> objects;

    // Vectors for ID values
    std::vector<GLuint> elementBuffers;
    std::vector<GLuint> vertexArrays;
    std::vector<GLuint> vertexBuffers;
    std::vector<GLuint> normalArrays;
    std::vector<GLuint> normalBuffers;
    
    // Camera that user controls
    camera * mainCamera;

    // SkyBox container
    skyBox * mainSkyBox;

    // Player agent
    bool aiMode;
    agent * userAgent;
    game_state * gameState;
    long long int trailCount;

public:
    // Default constructor, calls initialize and sets running to false
    application();
    ~application();

    // Sets up the window and context, also initialization of GLEW
    void initialize();

    // Handles the start of the application
    void start();

    // Simple loop method that keeps the application running with processing of events and calls render
    void loop();
    
    // Perform the necessarry rendering
    void render(double ctime, double ltime);

    /* Defined in input.cpp */
    // Handler for general events
    // Input: event - pointer to SDL_Event type
    //        deltaTime - time since last frame render, useful if input relies on relative to last frame
    // Output: identifies a relevant event and calls appriopriate metehod
    void event(SDL_Event * event, double deltaTime);

    // Handler for discrete keyboard presses - https://wiki.libsdl.org/SDL_Scancode was useful in identifying specific keys wanted
    // Input: key_event - SDL keyboard event object
    // Output: Depending on recognized key press, changes the mode for transformation behavior the application will do in render()
    void onKeyPress(SDL_KeyboardEvent * key_event, double deltaTime);    
    
    // Handler for keyboard presses that are discrete presses (like WASD movement)
    // Input: checks status for various keys
    // Output: responds accordingly
    void continuousKeyInput(double deltaTime);
    /* End of input.cpp */

    // Simple getter for getting running boolean value
    bool isRunning();
    
    // Handle closing the application
    void close();

    void updateShipLocation(double deltaTime);

    /* Defined in gameTick.cpp */
    // App tick to call what should happen after render and before next
    // Input: time elapsed in seconds
    // Output: Return true if a reset in gamestate happened
    bool tick(double deltaTime);

    // Set the positions and other similar values for the start of a run
    // Should set the ship back to start of the run cylinder, and also reposition the asteroids
    // Input: The seed that impacts the location of asteroids
    void setupScenario(unsigned int scenario_seed = 2);//time(NULL));
    /* End of gameTick.cpp */

    // Getter for agent instance
    const agent * getAgentInstance();

    // Simple getter for asteroid data (constant)
    const std::vector<Asteroid> getAsteroids();
    
    // Setter for if AI or manual mode, will only work if app not running
    void setAiMode(bool setAi);
};

#endif
