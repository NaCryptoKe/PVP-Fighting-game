#ifndef CHARACTER_STATE_H
#define CHARACTER_STATE_H

#include "utils/AABB.h"
#include <string>

enum class CharacterState
{
    IDLE,
    WALK,
    RUN,
    JUMP,
    ATTACK,
    BLOCK,          // standing guard
    CROUCH_BLOCK,   // crouching guard
    HITSTUN,
    DEATH,
    CROUCH,
    VICTORY         // round/match won — plays the celebration clips
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
    float hitstunTime = 0.3f;   // how long the victim stays in hitstun
    bool blockable = true;
    AttackType type;
};

// Everything the combat resolver needs to apply one hit
struct HitImpact
{
    float damage = 0.0f;
    float knockback = 0.0f;
    bool pushRight = true;
    float hitstunTime = 0.3f;
    bool blockable = true;
    std::string animOverride;   // e.g. "BACKWARD_THROW" when thrown
};

#endif  // CHARACTER_STATE_H
