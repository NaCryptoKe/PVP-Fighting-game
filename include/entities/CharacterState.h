#ifndef CHARACTER_STATE_H
#define CHARACTER_STATE_H

#include "utils/AABB.h"

enum class CharacterState
{
    IDLE,
    WALK,
    RUN,
    JUMP,
    ATTACK,
    BLOCK,
    HITSTUN,
    DEATH,
    CROUCH
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
    BOX hitBox;
    int startupFrame;
    int activeFrame;
    int recoveryFrame;
    float damageAmount;
    float knockBackForce = 0.0f;
    bool blockable = true;
    AttackType type;
};

#endif  // CHARACTER_STATE_H