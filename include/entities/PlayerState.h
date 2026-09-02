#ifndef PLAYER_STATE_H
#define PLAYER_STATE_H

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

#endif  // PLAYER_STATE_H