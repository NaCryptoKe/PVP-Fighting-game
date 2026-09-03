#include "entities/Player.h"

#include "GL/glut.h"
#include <iostream>

// Constructor
Player::Player(float posX, float posY, float HP)
    :   positionX(posX), 
        positionY(posY), 
        velocityX(0.0f), 
        velocityY(0.0f),
        maxHealth(HP), 
        currentHealth(HP),
        facingRight(true), 
        currentState(PlayerState::IDLE) {}

// Core game loops
void Player::update(float deltaTime)
{
    float groundLevel = 120.0f;

    const float gravity = 7349.94f;

    if (positionY > groundLevel)
    {
        float currentGravity = gravity;

        if (velocityY < 0.0f) { currentGravity *= 1; }
        velocityY -= currentGravity * deltaTime;
    }

    positionX += velocityX * deltaTime;
    positionY += velocityY * deltaTime;

    if (positionY < groundLevel)
    {
        positionY = groundLevel;
        velocityY = 0.0f;

        if (currentState == PlayerState::JUMP) currentState = PlayerState::IDLE;
    }

    moveHitbox();
}

void Player::render()
{
    glPushMatrix();

    // Placheholders
    float length = 300.0f;

    glTranslatef(positionX, positionY, 0.0f);

    glBegin(GL_QUADS);
        switch (currentState)
        {
            case PlayerState::IDLE:
                glColor3f(0.0f, 1.0f, 0.0f); // Green for IDLE
                break;
            case PlayerState::WALK:
                glColor3f(0.0f, 0.0f, 1.0f); // Blue for WALK
                break;
            case PlayerState::RUN:
                glColor3f(1.0f, 0.0f, 0.0f); // Red for RUN
                break;
            case PlayerState::JUMP:
                glColor3f(1.0f, 1.0f, 0.0f); // Yellow for JUMP
                break;
            case PlayerState::ATTACK:
                glColor3f(1.0f, 0.5f, 0.0f); // Orange for ATTACK
                break;
            case PlayerState::BLOCK:
                glColor3f(0.5f, 0.5f, 0.5f); // Grey for BLOCK
                break;
        }

        glVertex2f(-length / 2.0f, 0.0f);
        glVertex2f( length / 2.0f, 0.0f);
        glVertex2f( length / 2.0f, length);
        glVertex2f(-length / 2.0f, length);
        // Midpoint is located at the center of the feet
    glEnd();

    glPopMatrix();
}

void Player::renderHitBox()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(hitbox.playerBox.left, hitbox.playerBox.top);
        glVertex2f(hitbox.playerBox.left, hitbox.playerBox.bottom);
        glVertex2f(hitbox.playerBox.right, hitbox.playerBox.bottom);
        glVertex2f(hitbox.playerBox.right, hitbox.playerBox.top);
    glEnd();
}

void Player::takeDamage(float damage)
{
    if (currentState == PlayerState::BLOCK) {
        damage /= 4; // Reduce damage when blocking
    }

    currentHealth -= damage;
    if (currentHealth <= 0) 
    {
        currentHealth = 0;
        // Player is knocked out
    } 
    else { }// Not knocked out, deduct health
}

// Movement methods
void Player::moveFront() 
{
    facingRight ? velocityX = std::abs(walk) : velocityX = -std::abs(walk);
    if (isGrounded()) currentState = PlayerState::WALK;
}

void Player::moveBack() 
{
    !facingRight ? velocityX = std::abs(walk) : velocityX = -std::abs(walk);
    if (isGrounded()) currentState = PlayerState::WALK;
}

void Player::stopX() 
{
    velocityX = 0.0f;
    currentState = PlayerState::IDLE;
}

void Player::jump() 
{
    if (isGrounded())
    {
        velocityY = jumpForce;
        currentState = PlayerState::JUMP;
    }
}

void Player::autoFace(float opponentX)
{
    if (positionX <= opponentX) facingRight = true;
    else facingRight = false;
}

void Player::moveHitbox()
{
    hitbox.playerBox = hitbox.toWorld(positionX, positionY, facingRight);
}

// Setters
void Player::setPositionX(float posX) { positionX = posX; }
void Player::setPositionY(float posY) { positionY = posY; }
void Player::setVelocityX(float velX) { velocityX = velX; }
void Player::setVelocityY(float velY) { velocityY = velY; }
void Player::setFacingRight(bool isFacingRight) { facingRight = isFacingRight; }
void Player::setState(PlayerState state) { currentState = state; }
void Player::setHitBox(float offsetX, float offsetY, float width, float height) 
{ 
    hitbox.offsetX = offsetX;
    hitbox.offsetY = offsetY;
    hitbox.height = height;
    hitbox.width = width;

    hitbox.playerBox = hitbox.toWorld(offsetX, offsetY, facingRight);
}

// Getters
float Player::getPositionX() const { return positionX; }
float Player::getPositionY() const { return positionY; }
float Player::getVelocityX() const { return velocityX; }
float Player::getVelocityY() const { return velocityY; }
float Player::getHealth() const { return currentHealth; }
float Player::getMaxHealth() const { return maxHealth; }
bool Player::isFacingRight() const { return facingRight; }
bool Player::isGrounded() const { return positionY <= 120.0f; }
PlayerState Player::getState() const { return currentState; }
AABB Player::getHitBox() const { return hitbox.playerBox; }

// Temporary
void Player::collision(float leftLimit, float rightLimit)
{
    float playerRadius = 150.0f; // Half the player's visual width
    float padding = 50.0f;       // Distance from the exact screen edge

    // Check Right Wall
    if (positionX + playerRadius + padding >= rightLimit)
    {
        positionX = rightLimit - playerRadius - padding; 
        velocityX = 0.0f;
    }

    // Check Left Wall
    if (positionX - playerRadius - padding <= leftLimit)
    {
        positionX = leftLimit + playerRadius + padding;
        velocityX = 0.0f;
    }
}