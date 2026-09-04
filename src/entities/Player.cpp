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
        velocityY -= gravity * deltaTime;
    }

    const float friction = 8.0f;
    velocityX -= velocityX * friction * deltaTime;
    if (std::abs(velocityX) < 5.0f) velocityX = 0.0f;

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
        if (velocityX > 0.0f) velocityX = 0.0f;
    }

    // Check Left Wall
    if (positionX - playerRadius - padding <= leftLimit)
    {
        positionX = leftLimit + playerRadius + padding;
        velocityX = 0.0f;
        if (velocityX < 0.0f) velocityX = 0.0f;
    }
}

bool Player::loadAttack(
    AttackType type, 
    int startupFrame, 
    int activeFrame, 
    int recoveryFrame, 
    float damageAmount,
    float hboffsetX,
    float hboffsetY,
    float width,
    float height,
    const std::string &name,
    float knockBackForce,
    bool blockable
)
{
    AttackData attack;
    attack.type = type;

    attack.startupFrame = startupFrame;
    attack.activeFrame = activeFrame;
    attack.recoveryFrame = recoveryFrame;

    attack.damageAmount = damageAmount;

    attack.knockBackForce = knockBackForce;
    attack.blockable = blockable;

    BOX attackBox;
    attackBox.offsetX = hboffsetX;
    attackBox.offsetY = hboffsetY;
    attackBox.width = width;
    attackBox.height = height;

    attack.hitBox = attackBox;

    attacks[name] = attack;

    return true;
}

void Player::renderDamageBox(const std::string &name)
{
    printf("Is attack active: %s\n", isActiveAttack() ? "YES" : "NO");
    if (isActiveAttack())
    {
        auto it = attacks.find(name);
        if (it == attacks.end()) return; // Safe lookup without mutating map

        BOX damage = it->second.hitBox;
        damage.playerBox = damage.toWorld(positionX, positionY, facingRight);
        AABB damageBox = damage.playerBox;
        
        glColor3f(0.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
            glVertex2f(damageBox.left, damageBox.top);
            glVertex2f(damageBox.left, damageBox.bottom);
            glVertex2f(damageBox.right, damageBox.bottom);
            glVertex2f(damageBox.right, damageBox.top);
        glEnd();
    }
}

void Player::performAttack(const std::string &name)
{
    if (currentState == PlayerState::ATTACK)
    {
        return;
    }

    if (!isGrounded())
    {
        return;
    }

    auto it = attacks.find(name);
    if (it == attacks.end())
    {
        for (const auto &pair : attacks) { std::cout << "'" << pair.first << "' "; }
        std::cout << "\n";
        return;
    }

    currentAttackName = name;
    currentState = PlayerState::ATTACK;

    frameCounter = 0;
    frameAccumulator = 0.0f;
    hasHit = false;

    velocityX = 0.0f;
}

std::string Player::getCurrentAttackName() const
{
    return currentAttackName;
}

void Player::updateAttack(float deltaTime)
{
    if (currentState != PlayerState::ATTACK || currentAttackName.empty()) return;

    const AttackData &data = attacks.at(currentAttackName);

    frameAccumulator += deltaTime;
    const float frameDuration = 1.0f / 60.0f;   // ~0.01667 seconds/60FPS

    while (frameAccumulator >= frameDuration)
    {
        frameCounter++;
        frameAccumulator -= frameDuration;
    }

    int startupEnd = data.startupFrame;
    int activeEnd = data.activeFrame + startupEnd;
    int totalFrames = data.recoveryFrame + activeEnd;

    if (frameCounter <= startupEnd)
    {
        // PHASE 1: STARTUP (Wind-up)
        // Character prepares the move. Hitbox is OFF.
    }
    else if (frameCounter <= activeEnd)
    {
        // PHASE 2: ACTIVE (Damage Window)
        // Hitbox is ON in world space. Collisions are tested during this window.
        moveHitbox();
    }
    else if (frameCounter <= totalFrames)
    {
        // PHASE 3: RECOVERY (Cool-down)
        // Character resets to neutral stance. Hitbox is OFF. Player cannot act.
    }
    else
    {
        // PHASE 4: COMPLETE
        // Reset player back to IDLE state and clear attack variables
        currentState = PlayerState::IDLE;
        currentAttackName = "";
        frameCounter = 0;
        frameAccumulator = 0.0f;
        hasHit = false;
    }
    
}

bool Player::isActiveAttack() const
{
    if (currentState != PlayerState::ATTACK || currentAttackName.empty()) return false;

    auto it = attacks.find(currentAttackName);
    if (it == attacks.end()) return false;

    const AttackData &data = it->second;

    int startupEnd = data.startupFrame;
    int activeEnd = data.activeFrame + startupEnd;

    return (frameCounter > startupEnd && frameCounter <= activeEnd);
}

bool Player::getActiveAttackHitbox(AABB &outBox) const
{
    if (!isActiveAttack()) return false;

    auto it = attacks.find(currentAttackName);
    if (it == attacks.end()) return false;

    outBox = it->second.hitBox.toWorld(positionX, positionY, facingRight);
    return true;
}

float Player::getCurrentAttackDamage() const
{
    auto it = attacks.find(currentAttackName);
    return (it != attacks.end()) ? it->second.damageAmount : 0.0f;
}

float Player::getCurrentAttackKnockback() const
{
    auto it = attacks.find(currentAttackName);
    return (it != attacks.end()) ? it->second.knockBackForce : 0.0f;
}