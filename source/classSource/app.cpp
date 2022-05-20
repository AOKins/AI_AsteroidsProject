#ifndef _APP_CPP_
#define _APP_CPP_
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "app.hpp"
#include "../functions/graphicsMethods.cpp"


// Default constructor, calls initialize and sets running to false
application::application() {
    this->running = false;
    // Default screen resolution is 
    this->window_width = 760;
    this->window_height = 760;
    
    this->fps = 60; // Capping framerate to 60
    this->lineWidth = 1;
    this->wireFrameMode = false;
    this->renderSkybox = false;
    this->simGraphicsStepSize = 1;
    this->aiStepCount = 1;
    this->gameState = nullptr;
    this->aiMode = true;
    this->userAgent = nullptr;
    this->trailCount = 0;
    // Call initialize
    initialize();
}

// Sets up the window and context, also initialization of GLEW and SDL
void application::initialize() { 
    // Create window for app
    this->window = SDL_CreateWindow("Asteriod Dodger Simulation", 10, 100, window_width, window_height, SDL_WINDOW_OPENGL);
    if (window == NULL) {
        printf("ERROR::APPLICATION\nDid not successfully create window\n");
        exit(EXIT_FAILURE);
    }

    // Creating context for window
    this->context = SDL_GL_CreateContext(window);
    if (context == NULL) {
        printf("ERROR::APPLICATION\nDid not successfully create context\n");
        exit(EXIT_FAILURE);
    }
    
    // Initialize glew library
    if (glewInit() != GLEW_OK) {
        printf("ERROR::APPLICATION\nFailed to initialize GLEW\n");
        exit(EXIT_FAILURE);
    }
}

// Start the application, ending with call to loop()
void application::start() {
    this->running = true;
    // Creating shaders (uses files in ./shaders folder)
    // Objects //
    #ifdef DEBUG_BUILD
        this->objectsShader = new shader("./shaders/vertex.shader","./shaders/fragment.shader");
    #else
        this->objectsShader = new shader();
    #endif
    
    // SkyBox //
    this->mainSkyBox = new skyBox("./shaders/skyCube_vertex.shader", "./shaders/skyCube_fragment.shader", "./resources/Skycube/");

    // Object Stuff //
    // Demo spheres to give more reference points to show light effects
    this->objects.clear();
    this->elementBuffers.clear();
    this->vertexArrays.clear();
    this->vertexBuffers.clear();

    // Load objects
    // Load Ship
    this->objects.push_back(object_gl("resources/sphere.obj"));
    
    // Load bounds representation
    this->objects.push_back(object_gl("resources/cylinder_bounds.obj"));

    // Load asteroids
    for (int i = 0; i <= ASTEROID_COUNT; i++) {
        this->objects.push_back(object_gl("resources/sphere.obj"));
    }
    
    // Reserving space
    this->elementBuffers.reserve(this->objects.size());
    this->vertexArrays  .reserve(this->objects.size());
    this->vertexBuffers .reserve(this->objects.size());

    // Load the objects into OpenGL
    for (int i = 0; i < this->objects.size(); i++) {
        GLmethods::load_object(this->objectsShader, &this->objects[i], &elementBuffers[i], &vertexArrays[i], 
                                 &vertexBuffers[i]);
    }

    // Setup agent to control the ship
    this->mainCamera = new thirdPerson_camera(&this->objects[0]);
    // Correcting main camera to the aspect ratio of the window
    mainCamera->setAspect(float(window_width)/float(window_height));
    mainCamera->setClipFar(300.0f);
    
    this->mainCamera->setHeading(-M_PI/2.0f);
    this->setupScenario();
    
    // Call the loop method now that initial setup is done
    loop();
}

// Setter for if AI or manual mode, will only work if app not running
void application::setAiMode(bool setAi) {
    if (this->running == false)
        this->aiMode = setAi;
}

// Core event/render loop
void application::loop() {
    SDL_Event event;
    Uint32 currTime,lastTime;
    double delta;
    // Last time is currently now
    lastTime = SDL_GetTicks();
    bool reset = false;
    while (this->running) {
        // Get current time in milliseconds
        currTime = SDL_GetTicks();
        
        if (reset) {
            lastTime = currTime;
            reset = false;
        }

        delta = currTime - lastTime;

        if (delta > 1000/double(this->fps)) {
            // Process events that occurred since last time
            while(SDL_PollEvent(&event)) {
                this->event(&event, currTime - lastTime);
            }
            render(currTime, lastTime);

            for (int i = 0; i < this->simGraphicsStepSize && !reset; i++) {
                reset = tick(delta/1000.0f);
            }
            this->continuousKeyInput(delta/1000.0f);
            this->mainCamera->realtimeUpdate();
                // Before entering new loop, this time is now last time
            lastTime = currTime;
        }
        else {
            // Wait if not time yet (with 1ms to spare)
            SDL_Delay(1000/double(this->fps) - delta-1);
        }
    }
}

// Handles the actual rendering behavior (including manage of triangle data)
void application::render(double ctime, double ltime) {
    // Render the scene normally //
    glViewport(0,0, this->window_width, this->window_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

        // Using the one object shader program created in setup, identifying with the id value in the app's struct
    glUseProgram(objectsShader->shaderID);

    // Setting camera transform
    objectsShader->setMat4("camera", mainCamera->getView());
    // Setting perspective transform
    objectsShader->setMat4("perspective", mainCamera->getPerspective());
    // Setting camera position for lighting
    objectsShader->setVec3("translation", mainCamera->getPosition());
    
    // Render the objects
    for (int i = 0; i < objects.size();i++) {
        GLmethods::render_object(this->objectsShader, &this->objects[i], this->vertexBuffers[i]);
    }

    if (this->renderSkybox) {
        // Render the skybox
        glUseProgram(this->mainSkyBox->skyBoxShader->shaderID);
        this->mainSkyBox->renderSkyBox(mainCamera->getPerspective(), mainCamera->getProjection());
    }
    // Swapping buffers to update display
    SDL_GL_SwapWindow(window);
}

// Simple getter for running property
bool application::isRunning(){
    return this->running;
}

// Handle closing the application
void application::close() {
    this->running = false;
    GLmethods::delete_arrays(this->vertexArrays);
    // Closing window and closing library
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Simple deconstructor that deletes the shader and triangle data
application::~application() {
    // Clear up everything
    delete this->objectsShader;
    delete this->mainSkyBox;
    delete this->mainCamera;
    delete this->gameState;
    delete this->userAgent;
}

#endif
