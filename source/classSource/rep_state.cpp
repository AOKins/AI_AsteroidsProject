#ifndef _REP_STATE_CPP_
#define _REP_STATE_CPP_
#include "rep_state.hpp"

// Constructors //
rep_state::rep_state() {
    this->present_state = nullptr;
    this->act_ref = nullptr;
    this->proj_agentPos = glm::vec3(0);
    this->proj_agentVel = glm::vec3(0);
    this->stateScore = nan(nullptr);
    this->type = ARENA;
    this->agentCollisionRadius = -1;
}

rep_state::rep_state(const game_state * ref) {
    this->present_state = ref;
    this->act_ref = ref->getActorRef();
    const agent * userAgent = ref->getAgentRef();
    this->proj_agentPos = userAgent->getPosition();
    this->proj_agentVel = userAgent->getVelocity();
    this->stateScore = ref->determineScore();
    this->type = ref->getStateType();
    this->agentCollisionRadius = userAgent->getCollisionRadius();
}

// Constructor takes in game state, agent, and action (with duration) to project what expected resulting state may be
rep_state::rep_state(const game_state * ref, const agent * userAgent, gameLogic::ACTION action, double deltaTime) {
    this->present_state = ref;
    this->act_ref = userAgent->getActorRef();
    this->proj_agentPos = userAgent->getPosition();
    this->proj_agentVel = userAgent->getVelocity();
    glm::vec3 accel = gameLogic::netAccelerationShip(userAgent,ref->getRefAsteroidData());
    this->stateScore = nan(nullptr);
    this->agentCollisionRadius = userAgent->getCollisionRadius();

    glm::vec3 resultVelChange = glm::vec3(0);
    switch (action) {
        default:
        case gameLogic::ACTION::HOLD:
            break;
        case gameLogic::ACTION::POS_X:
            resultVelChange.x = this->act_ref->projDelta(0,deltaTime);
            break;
        case gameLogic::ACTION::NEG_X:
            resultVelChange.x = this->act_ref->projDelta(0,deltaTime);
            break;
        case gameLogic::ACTION::POS_Y:
            resultVelChange.x = this->act_ref->projDelta(0,deltaTime);
            break;
        case gameLogic::ACTION::NEG_Y:
            resultVelChange.x = this->act_ref->projDelta(0,deltaTime);
            break;
        case gameLogic::ACTION::POS_Z:
            resultVelChange.x = this->act_ref->projDelta(0,deltaTime);
            break;
        case gameLogic::ACTION::NEG_Z:
            resultVelChange.x = this->act_ref->projDelta(0,deltaTime);
            break;

    }

    this->proj_agentPos.x += this->proj_agentVel.x*deltaTime + 0.5*accel.x*deltaTime*deltaTime;
    this->proj_agentPos.y += this->proj_agentVel.y*deltaTime + 0.5*accel.y*deltaTime*deltaTime;
    this->proj_agentPos.z += this->proj_agentVel.z*deltaTime + 0.5*accel.z*deltaTime*deltaTime;

    this->proj_agentVel.x += resultVelChange.x+accel.x*deltaTime;
    this->proj_agentVel.y += resultVelChange.y+accel.y*deltaTime;
    this->proj_agentVel.z += resultVelChange.z+accel.z*deltaTime;

    this->type = ref->getStateType();
}
// End of Constructors //

// Getter for the game_state that this references (typically used just to then get access to asteroid positions)
const game_state* rep_state::getPresentState() const {
    return this->present_state;
}
// Getter for state type
STATE rep_state::getStateType() const {
    return this->type;
}
// Get what the score for this state is
double rep_state::getDetermineScore() const {
    return this->stateScore;
}
const double rep_state::getUserAgentCollisionRadius() const {
    return this->agentCollisionRadius;
}
const glm::vec3 rep_state::getProjPos() const {
    return this->proj_agentPos;
}
const glm::vec3 rep_state::getProjVel() const {
    return this->proj_agentVel;
}
double rep_state::getProjDelta() const {
    return this->proj_delta;
}
// Get what the expected resulting thrust acceleration would be
const double rep_state::getProjAccelAct(ACTION act, double deltaTime) const {
    switch (act) {
        case POS_X:
            return this->act_ref->projDelta(0, deltaTime);
            break;
        case POS_Y:
            return this->act_ref->projDelta(1, deltaTime);
            break;
        case POS_Z:
            return this->act_ref->projDelta(2, deltaTime);
            break;
        case NEG_X:
            return this->act_ref->projDelta(0,-deltaTime);
            break;
        case NEG_Y:
            return this->act_ref->projDelta(1,-deltaTime);
            break;
        case NEG_Z:
            return this->act_ref->projDelta(2,-deltaTime);
            break;
        case HOLD:
        default:
            return 0;
    }
};


#endif
