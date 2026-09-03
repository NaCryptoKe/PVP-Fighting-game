#ifndef PLAYER_H
#define PLAYER_H

#include "entities/PlayerState.h"
#include "utils/AABB.h"

class Player
{
private:
    // Transform and movement physics
    float positionX;
    float positionY;
    float velocityX;
    float velocityY;
    const float jumpForce = 2571.43f;
    const float walk = 450.0f;

    // Combat Health
    float maxHealth;
    float currentHealth;

    // Player orientation
    bool facingRight; // true - facing right, false - facing left

    // Player state
    PlayerState currentState;

    // Hurtbox and hitbox added here
    BOX hitbox;

public:
    Player(float posX, float posY, float HP);

    // Core game loops
    void update(float deltaTime); // Update method to handle state changes and other changes
    void render(); // Draw the player
    void renderHitBox();

    void takeDamage(float damage);

    // Movement methods
    void moveFront();
    void moveBack();
    void stopX();
    void jump();
    void autoFace(float opponentX);
    void moveHitbox();

    // Setters
    void setPositionX(float posX);
    void setPositionY(float posY);
    void setVelocityX(float velX);
    void setVelocityY(float velY);
    void setFacingRight(bool facingRight);
    void setState(PlayerState state);
    void setHitBox(float offsetX, float offsetY, float width, float height);

    // Getters
    float getPositionX() const;
    float getPositionY() const;
    float getVelocityX() const;
    float getVelocityY() const;
    float getHealth() const;
    float getMaxHealth() const;
    bool isFacingRight() const;
    bool isGrounded() const;
    PlayerState getState() const;
    AABB getHitBox() const;

    // Temporary
    void collision(float leftLimit, float rightLimit);
};

#endif // PLAYER_H