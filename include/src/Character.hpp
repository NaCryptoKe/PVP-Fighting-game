#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "src/Sprite.hpp"
#include "src/Animation.hpp"

enum class CharacterState 
{
    IDLE,
    WALKING,
    JUMPING,
    CROUCHING,
    ATTACKING,
    BLOCKING,
    HIT_STUN
};

class Character 
{
private:
    Sprite sprite;

    // Animation idleAnim; and others
    Animation jumpAnim;
    Animation idleAnim;
    Animation* currentAnim;

    float x, y;
    float velocityX, velocityY;
    bool facingRight;

    CharacterState currentState;

    int health;
    int maxHealth;

public:
    Character();

    bool init(
        const char* idleFolder, int idleFrames,
        const char* jumpFolder, int jumpFrames
    );

    void update(float deltaTime);
    void render();

    void setState(CharacterState newState);
    void setPosition(float px, float py);
    void setFacing(bool right);
    void setScale(int scale);

    float getX() const;
    float getY() const;
    int getHealth() const;
};

#endif // CHARACTER_HPP