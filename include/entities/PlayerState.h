#ifndef PLAYER_STATE_H
#define PLAYER_STATE_H

#include "utils/AABB.h"

enum class PlayerState
{
    IDLE,
    WALK,
    RUN,
    JUMP,
    ATTACK,
    BLOCK
    // Implement others that might be necessary in the future, like crouch, KO, etc.
};

enum class AttackType 
{
    LIGHT_PUNCH,
    LIGHT_KICK,

    HARD_PUNCH,
    HARD_KICK
};

struct AttackData
{
    // Animation gets added here

    BOX hitBox;

    int startupFrame;
    int activeFrame;
    int recoveryFrame;

    float damageAmount;

    float knockBackForce = 0.0f;
    bool blockable = true;

    AttackType type;
};

#endif  // PLAYER_STATE_H