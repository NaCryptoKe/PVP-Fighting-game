#include "entities/Player.h"

#include "GL/glut.h"
#include "render/Renderer.h"
#include <iostream>
#include <cmath>

Player::Player(float posX, float posY, float HP)
    :   positionX(posX), positionY(posY), velocityX(0.0f), velocityY(0.0f),
        maxHealth(HP), currentHealth(HP),
        facingRight(true), currentState(PlayerState::IDLE) {}

void Player::update(float deltaTime)
{
    float groundLevel = 120.0f;
    const float gravity = 7349.94f;

    if (positionY > groundLevel)
    {
        velocityY -= gravity * deltaTime;
    }

    // Knockback decay — only while stunned, so normal walk speed is untouched
    if (currentState == PlayerState::HITSTUN)
    {
        const float friction = 8.0f;
        velocityX -= velocityX * friction * deltaTime;
        if (std::abs(velocityX) < 5.0f) velocityX = 0.0f;
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
    // If sprite has a valid texture, render it; otherwise use fallback colored quad
    if (sprite.getTexture().id != 0)
    {
        sprite.setPosition(positionX, positionY);
        sprite.setFlip(!facingRight);
        sprite.draw();
    }
    else
    {
        // Fallback to colored quad for development
        glPushMatrix();
        float length = 300.0f;
        glTranslatef(positionX, positionY, 0.0f);

        glBegin(GL_QUADS);
            switch (currentState)
            {
                case PlayerState::IDLE:    glColor3f(0.0f, 1.0f, 0.0f); break;
                case PlayerState::WALK:    glColor3f(0.0f, 0.0f, 1.0f); break;
                case PlayerState::RUN:     glColor3f(1.0f, 0.0f, 0.0f); break;
                case PlayerState::JUMP:    glColor3f(1.0f, 1.0f, 0.0f); break;
                case PlayerState::ATTACK:  glColor3f(1.0f, 0.5f, 0.0f); break;
                case PlayerState::BLOCK:   glColor3f(0.5f, 0.5f, 0.5f); break;
                case PlayerState::HITSTUN: glColor3f(1.0f, 0.0f, 1.0f); break;
                case PlayerState::DEATH:   glColor3f(0.1f, 0.1f, 0.1f); break;
                case PlayerState::CROUCH:  glColor3f(0.55f, 0.27f, 0.07f); break;
            }
            glVertex2f(-length / 2.0f, 0.0f);
            glVertex2f( length / 2.0f, 0.0f);
            glVertex2f( length / 2.0f, length);
            glVertex2f(-length / 2.0f, length);
        glEnd();

        glPopMatrix();
    }
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
    if (currentState == PlayerState::BLOCK) damage /= 4.0f;
    currentHealth -= damage;
    if (currentHealth <= 0.0f) currentHealth = 0.0f;
}

void Player::onHit(float damage, float knockback, bool pushRight, float hitstunTime)
{
    bool wasBlocking = (currentState == PlayerState::BLOCK);
    takeDamage(damage);

    if (currentHealth <= 0.0f)
    {
        currentState = PlayerState::DEATH;
        velocityX = 0.0f;
        return;
    }

    if (!wasBlocking)
    {
        velocityX = pushRight ? knockback : -knockback;
        currentState = PlayerState::HITSTUN;
        hitstunTimer = hitstunTime;
    }
}

void Player::updateHitstun(float deltaTime)
{
    if (currentState != PlayerState::HITSTUN) return;

    hitstunTimer -= deltaTime;
    if (hitstunTimer <= 0.0f)
    {
        hitstunTimer = 0.0f;
        currentState = PlayerState::IDLE;
    }
}

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
    facingRight = (positionX <= opponentX);
}

void Player::moveHitbox()
{
    hitbox.playerBox = hitbox.toWorld(positionX, positionY, facingRight);
}

void Player::setBlocking(bool wantBlock)
{
    if (wantBlock)
    {
        if (canAct()) currentState = PlayerState::BLOCK;
    }
    else if (currentState == PlayerState::BLOCK)
    {
        currentState = PlayerState::IDLE;
    }
}

void Player::setCrouching(bool wantCrouch)
{
    if(wantCrouch)
    {
        if (canAct() && isGrounded()) currentState = PlayerState::CROUCH;
    }
    else if (currentState == PlayerState::CROUCH)
    {
        currentState = PlayerState::IDLE;
    }
}

bool Player::isBlocking() const { return currentState == PlayerState::BLOCK; }

bool Player::canAct() const
{
    return currentState != PlayerState::ATTACK &&
           currentState != PlayerState::HITSTUN &&
           currentState != PlayerState::DEATH;
}

bool Player::canMove() const
{
    return canAct() && currentState != PlayerState::BLOCK;
}

bool Player::isDead() const { return currentState == PlayerState::DEATH; }

void Player::resetForRound(float posX, float posY)
{
    positionX = posX;
    positionY = posY;
    velocityX = 0.0f;
    velocityY = 0.0f;
    currentHealth = maxHealth;
    currentState = PlayerState::IDLE;
    currentAttackName = "";
    frameCounter = 0;
    frameAccumulator = 0.0f;
    hasHit = false;
    hitstunTimer = 0.0f;
    moveHitbox();
}

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

bool Player::loadAttack(
    AttackType type, int startupFrame, int activeFrame, int recoveryFrame,
    float damageAmount, float hboffsetX, float hboffsetY, float width, float height,
    const std::string &name, float knockBackForce, bool blockable)
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
    if (!isActiveAttack()) return;
    auto it = attacks.find(name);
    if (it == attacks.end()) return;

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

void Player::performAttack(const std::string &name)
{
    if (!canAct()) return;
    if (currentState == PlayerState::BLOCK) return;
    if (!isGrounded()) return;

    auto it = attacks.find(name);
    if (it == attacks.end()) return;

    currentAttackName = name;
    currentState = PlayerState::ATTACK;
    frameCounter = 0;
    frameAccumulator = 0.0f;
    hasHit = false;
    velocityX = 0.0f;
}

std::string Player::getCurrentAttackName() const { return currentAttackName; }

void Player::updateAttack(float deltaTime)
{
    if (currentState != PlayerState::ATTACK || currentAttackName.empty()) return;

    const AttackData &data = attacks.at(currentAttackName);

    frameAccumulator += deltaTime;
    const float frameDuration = 1.0f / 60.0f;
    while (frameAccumulator >= frameDuration)
    {
        frameCounter++;
        frameAccumulator -= frameDuration;
    }

    int startupEnd = data.startupFrame;
    int activeEnd = data.activeFrame + startupEnd;
    int totalFrames = data.recoveryFrame + activeEnd;

    if (frameCounter > totalFrames)
    {
        currentState = PlayerState::IDLE;
        currentAttackName = "";
        frameCounter = 0;
        frameAccumulator = 0.0f;
        hasHit = false;
    }

    // Update animation if it exists for this attack state
    auto animIt = animations.find(currentAttackName);
    if (animIt != animations.end())
    {
        animIt->second.update(deltaTime);
        sprite.setTexture(animIt->second.getCurrentTexture());
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

bool Player::getHasHit() const { return hasHit; }
void Player::markHit() { hasHit = true; }

void Player::collision(float leftLimit, float rightLimit)
{
    float playerRadius = 150.0f;
    float padding = 50.0f;

    if (positionX + playerRadius + padding >= rightLimit)
        positionX = rightLimit - playerRadius - padding;

    if (positionX - playerRadius - padding <= leftLimit)
        positionX = leftLimit + playerRadius + padding;
}