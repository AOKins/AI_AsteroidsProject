// Extension of app.cpp for methods relating to app events/inputs
#ifndef _INPUT_CPP_
#define _INPUT_CPP_

#include "app.hpp"

// Handler for general events
// Input: event - pointer to SDL_Event type
//        deltaTime - time since last frame render, useful if input relies on relative to last frame
// Output: identifies a relevant event and calls appriopriate metehod
void application::event(SDL_Event * event, double deltaTime) {
    // Determine the event and call appriopriate method
    switch (event->type) {
        case (SDL_QUIT):
            close();
            break;
        case (SDL_KEYDOWN):
            onKeyPress(&event->key, deltaTime);
            break;
    }
}

// Handler for discrete keyboard presses - https://wiki.libsdl.org/SDL_Scancode was useful in identifying specific keys wanted
// Input: key_event - SDL keyboard event object
// Output: Depending on recognized key press, changes the mode for transformation behavior the application will do in render()
void application::onKeyPress(SDL_KeyboardEvent * key_event, double deltaTime) {
    switch(key_event->keysym.scancode) {
        case (SDL_SCANCODE_ESCAPE):
            this->close();
            break;
        case (SDL_SCANCODE_T):
            if (this->aiMode)
                static_cast<ai_agent*>(this->userAgent)->toggleTrainingMode();
            break;
        // Function keys change polygon mode between filled and lines (wireframe)
        case (SDL_SCANCODE_F1):
            this->wireFrameMode = !this->wireFrameMode;
            if (this->wireFrameMode) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);        
            }
            else  {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            break;
        // Adjust if skybox gets rendered or not
        case (SDL_SCANCODE_F2):
            this->renderSkybox = !this->renderSkybox;
            break;
        // Adjust FOV of the camera
        case (SDL_SCANCODE_F5):
            mainCamera->setFOV(mainCamera->getFOV()-0.01f);
            break;
        case (SDL_SCANCODE_F6):
            mainCamera->setFOV(mainCamera->getFOV()+0.01f);
            break;
        case (SDL_SCANCODE_MINUS):
            glLineWidth(this->lineWidth > 1 ? --this->lineWidth : this->lineWidth);
            break;
        case (SDL_SCANCODE_EQUALS):
            glLineWidth(++this->lineWidth);
            break;
        case (SDL_SCANCODE_F9):
            printf("Current TimeScaling %i\n",this->simGraphicsStepSize > 1 ? this->simGraphicsStepSize/=2 : this->simGraphicsStepSize);
            break;
        case (SDL_SCANCODE_F10):
            printf("Current TimeScaling %i\n", this->simGraphicsStepSize *= 2);
            break;
        case (SDL_SCANCODE_F11):
            printf("Current Ai stepping count per frame %i\n",this->aiStepCount > 1 ? --this->aiStepCount : this->aiStepCount);
            break;
        case (SDL_SCANCODE_F12):
            printf("Current Ai stepping count per frame %i\n", ++this->aiStepCount);
            break;
        default: // If unrecognized, default to doing nothing
            break;
    }
}

// Handler for keyboard presses that are discrete presses (like WASD movement)
// Input: checks status for various keys
// Output: responds accordingly
void application::continuousKeyInput(double deltaTime) {
    const Uint8 * keyboardState = SDL_GetKeyboardState(NULL);
    if (keyboardState[SDL_SCANCODE_UP] == SDL_PRESSED)    { this->mainCamera->turnUp(    deltaTime);    }
    if (keyboardState[SDL_SCANCODE_DOWN] == SDL_PRESSED)  { this->mainCamera->turnUp(   -deltaTime);    }
    if (keyboardState[SDL_SCANCODE_LEFT] == SDL_PRESSED)  { this->mainCamera->turnRight( deltaTime);    }
    if (keyboardState[SDL_SCANCODE_RIGHT] == SDL_PRESSED) { this->mainCamera->turnRight( -deltaTime);   }
    if (keyboardState[SDL_SCANCODE_RALT] == SDL_PRESSED)  { this->mainCamera->moveFoward( 10*deltaTime);}
    if (keyboardState[SDL_SCANCODE_RCTRL] == SDL_PRESSED) { this->mainCamera->moveFoward(-10*deltaTime);}
}

#endif
