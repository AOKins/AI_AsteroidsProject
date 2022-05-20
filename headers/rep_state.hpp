#ifndef _REF_STATE_HPP_
#define _REF_STATE_HPP_
#include <GLM/glm.hpp>

#include "game_state.hpp"

// ref_state is a projection forward/backward of a given game state
class rep_state {
    const game_state * present_state;
    const actor * act_ref;
    glm::vec3 proj_agentPos;
    glm::vec3 proj_agentVel;
    double agentCollisionRadius;
    double proj_delta;
    double stateScore;
    gameLogic::STATE type;
public:
    // Constructors
    rep_state();
    rep_state(const game_state * ref);
    // Constructor takes in game state, agent, and action (with duration) to project what expected resulting state may be
    rep_state(const game_state * ref, const agent * userAgent, gameLogic::ACTION action, double deltaTime);

    // Getter for the game_state that this references (typically used just to then get access to asteroid positions)
    const game_state* getPresentState() const;
    // Getter for state type
    STATE getStateType() const;
    // Get what the score for this state is
    double getDetermineScore() const;
    const double getUserAgentCollisionRadius() const;
    const glm::vec3 getProjPos() const;
    const glm::vec3 getProjVel() const;
    double getProjDelta() const;
    // Get what the expected resulting thrust acceleration would be
    const double getProjAccelAct(ACTION act, double deltaTime) const;
};

#endif
