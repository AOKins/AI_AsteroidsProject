// Extension of app.cpp for more methods, most notably the implementation fo application::tick()
#ifndef _TICK_CPP_
#define _TICK_CPP_
#include "app.hpp"
#include "../functions/gameLogic.cpp"

// Set the positions and other similar values for the start of a run
// Should set the ship back to start of the run cylinder, and also reposition the asteroids
// Input: The seed that impacts the location of asteroids
void application::setupScenario(unsigned int scenario_seed) {
    if (this->gameState != nullptr) {
        delete this->gameState;
    }
    if (this->userAgent == nullptr) {
        this->userAgent = this->aiMode ? new ai_agent(&this->objects[0], this->gameState) : new agent(&this->objects[0]);
    }
    this->gameState = new game_state(this->userAgent, scenario_seed, this->objects.data());
    if (this->aiMode) {
        static_cast<ai_agent*>(this->userAgent)->setState(this->gameState);
    }
}

// App tick to call what should happen after render and before next
// Input: time elapsed in seconds
// Output: Return true if a reset in gamestate happened
bool application::tick(double deltaTime) {
    gameLogic::STATE s;
    // Doing a null check
    if (this->gameState != nullptr) {
       s = this->gameState->tick(deltaTime, this->aiStepCount);
    }
    else {
        this->setupScenario();
        return true;
    }

    // If the tick of the game state resulted in either EXIT or FAIL
    // If also not in AI mode, will need to messagebox the user
    if (s != gameLogic::STATE::ARENA) {
        if (this->aiMode == false) {
            double score = this->gameState->determineScore();
            switch (s) {
                case(gameLogic::STATE::EXIT):
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Successful exit state","Success",this->window);
                    std::cout << "Success! Score is " << score << std::endl;
                    break;
                case(gameLogic::STATE::FAIL):
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Invalid solution state","Wrong!",this->window);
                    std::cout << "Failure! Score is " << score << std::endl;
                    break;
                default:
                    break;
            }
        }
        else if (this->aiMode == true && s == gameLogic::STATE::EXIT) {
            std::cout << "Success! It found the exit\n\t Used delta: " << this->userAgent->getUsedDelta() << std::endl; 
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Successful exit state","Success",this->window);
        }
        this->setupScenario();
        return true;
    }
    return false;
}

#endif
