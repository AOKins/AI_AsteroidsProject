// Features available to be used by the AI agent
#ifndef _FEATURES_CPP_
#define _FEATURES_CPP_

#include "agent.hpp"
#include "gameLogic.cpp"
#include "rep_state.hpp"

using namespace gameLogic;

namespace agentFeatures {
    // Distance from the Z origin plane
    double DistanceFromZ(const rep_state * state, const ACTION act, const double deltaTime) {
        return state->getProjPos().z/(MAX_DISTANCE_TARGET);
    }
    // Distance from the X origin plane
    double XposMag(const rep_state * state, const ACTION act, const double deltaTime) {
        return abs(state->getProjPos().x)/(BOUNDS_R);
    }
    // Distance from the Y origin plane
    double YposMag(const rep_state * state, const ACTION act, const double deltaTime) {
        return abs(state->getProjPos().y)/(BOUNDS_R);
    }
    
    // Distance from the bounds (cylindrical region)
    double DistanceFromBounds(const rep_state * state, const ACTION act, const double deltaTime) {
        glm::vec3 pos = state->getProjPos();
        double distance = gameLogic::getDistance(glm::vec3(pos.x,pos.y,0),glm::vec3(BOUNDS_R,BOUNDS_R,0));
        return 1-(distance/(2000*BOUNDS_R));
    }

    // Distance from the closest asteroid surface
    double DistFromCloseSurf(const rep_state * state, const ACTION act, const double deltaTime) {
        
        const double agent_r = state->getUserAgentCollisionRadius();
        const glm::vec3 agent_pos = state->getProjPos();
        
        const game_state * gstate = state->getPresentState();
        const Asteroid* a0 = &gstate->getRefAsteroidData()[0];
        double closestSurfDist = gameLogic::getDistance(agent_pos, a0->getPosition()) - (agent_r + a0->getRadius());

        double aSurfDist;
        for (Asteroid a : gstate->getRefAsteroidData()) {
            aSurfDist = gameLogic::getDistance(agent_pos, a.getPosition()) - (agent_r + a.getRadius());
            
            closestSurfDist = aSurfDist < closestSurfDist ? aSurfDist : closestSurfDist;

        }
        return 1 - (aSurfDist /(2000*MAX_DISTANCE_TARGET));
    }

    // Amount of delta velocity used so far
    double DeltaUsed(const rep_state * state, const ACTION act, const double deltaTime) {
        return state->getProjDelta();
    }

    // Percentage of asteroids passed by along to the end (simply using Z position centers)
    double PropAsteroidsPassed(const rep_state * state, const ACTION act, const double deltaTime) {
        const game_state * gstate = state->getPresentState();
        const glm::vec3 agent_pos = state->getProjPos();
        const int astCount = gstate->getRefAsteroidData().size();
        int count = 0;
        for (Asteroid a : gstate->getRefAsteroidData()) {
            count = count + (agent_pos.z > a.getPosition().z);
        }
        return double(count)/astCount;
    }

    // Anticipated Velocity in Z axis
    double ZVelocity(const rep_state * state, const ACTION act, const double deltaTime) {
        double resV = state->getProjVel().z;
        if (act != POS_Z && act != NEG_Z) 
            return resV;

        double accel = state->getProjAccelAct(act,deltaTime);
        return (resV + accel);
    }
    // Anticipated Velocity in X axis
    double XVelocity(const rep_state * state, const ACTION act, const double deltaTime) {
        double resV = state->getProjVel().x;
        if (act != POS_X && act != NEG_X) 
            return resV/1e3;

        double accel = state->getProjAccelAct(act,deltaTime);
        return (resV + accel)/1e3;
    }
    
    // Anticipated Velocity in Y axis
    double YVelocity(const rep_state * state, const ACTION act, const double deltaTime) {
        double resV = state->getProjVel().y;
        if (act != POS_Y && act != NEG_Y) 
            return resV;

        double accel = state->getProjAccelAct(act,deltaTime);
        return (resV + accel)/1e3;
    }

    // Feature that ideally promotes choosing an action that either doesn't use fuel (hold) or moving to the end goal (pos-z)
    double DesiredAction(const rep_state * state, const ACTION act, const double deltaTime) {
        return act == ACTION::POS_Z || act==ACTION::HOLD;
    }
    // Opposed of DesiredAction() feature
    double UnDesiredAction(const rep_state * state, const ACTION act, const double deltaTime) {
        return !DesiredAction(state,act,deltaTime);
    }
};

#endif
