#include "entities/Character.h"

#include "GL/glut.h"
#include "render/Renderer.h"
#include <iostream>
#include <cmath>

Character::Character(float posX, float posY, float HP)
    :   positionX(posX), positionY(posY), velocityX(0.0f), velocityY(0.0f),
        maxHealth(HP), currentHealth(HP),
        facingRight(true), currentState(PlayerState::IDLE) {}

void Character::update(float deltaTime)
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

void Character::render()
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

void Character::renderHitBox()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(hitbox.playerBox.left, hitbox.playerBox.top);
        glVertex2f(hitbox.playerBox.left, hitbox.playerBox.bottom);
        glVertex2f(hitbox.playerBox.right, hitbox.playerBox.bottom);
        glVertex2f(hitbox.playerBox.right, hitbox.playerBox.top);
    glEnd();
}

void Character::takeDamage(float damage)
{
    if (currentState == PlayerState::BLOCK) damage /= 4.0f;
    currentHealth -= damage;
    if (currentHealth <= 0.0f) currentHealth = 0.0f;
}

void Character::onHit(float damage, float knockback, bool pushRight, float hitstunTime)
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

void Character::updateHitstun(float deltaTime)
{
    if (currentState != PlayerState::HITSTUN) return;

    hitstunTimer -= deltaTime;
    if (hitstunTimer <= 0.0f)
    {
        hitstunTimer = 0.0f;
        currentState = PlayerState::IDLE;
    }
}

void Character::moveFront()
{
    facingRight ? velocityX = std::abs(walk) : velocityX = -std::abs(walk);
    if (isGrounded()) currentState = PlayerState::WALK;
}

void Character::moveBack()
{
    !facingRight ? velocityX = std::abs(walk) : velocityX = -std::abs(walk);
    if (isGrounded()) currentState = PlayerState::WALK;
}

void Character::stopX()
{
    velocityX = 0.0f;
    currentState = PlayerState::IDLE;
}

void Character::jump()
{
    if (isGrounded())
    {
        velocityY = jumpForce;
        currentState = PlayerState::JUMP;
    }
}

void Character::autoFace(float opponentX)
{
    facingRight = (positionX <= opponentX);
}

void Character::moveHitbox()
{
    hitbox.playerBox = hitbox.toWorld(positionX, positionY, facingRight);
}

void Character::setBlocking(bool wantBlock)
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

void Character::setCrouching(bool wantCrouch)
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

bool Character::isBlocking() const { return currentState == PlayerState::BLOCK; }

bool Character::canAct() const
{
    return currentState != PlayerState::ATTACK &&
           currentState != PlayerState::HITSTUN &&
           currentState != PlayerState::DEATH;
}

bool Character::canMove() const
{
    return canAct() && currentState != PlayerState::BLOCK;
}

bool Character::isDead() const { return currentState == PlayerState::DEATH; }

void Character::resetForRound(float posX, float posY)
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

void Character::setPositionX(float posX) { positionX = posX; }
void Character::setPositionY(float posY) { positionY = posY; }
void Character::setVelocityX(float velX) { velocityX = velX; }
void Character::setVelocityY(float velY) { velocityY = velY; }
void Character::setFacingRight(bool isFacingRight) { facingRight = isFacingRight; }
void Character::setState(PlayerState state) { currentState = state; }
void Character::setHitBox(float offsetX, float offsetY, float width, float height)
{
    hitbox.offsetX = offsetX;
    hitbox.offsetY = offsetY;
    hitbox.height = height;
    hitbox.width = width;
    hitbox.playerBox = hitbox.toWorld(offsetX, offsetY, facingRight);
}

float Character::getPositionX() const { return positionX; }
float Character::getPositionY() const { return positionY; }
float Character::getVelocityX() const { return velocityX; }
float Character::getVelocityY() const { return velocityY; }
float Character::getHealth() const { return currentHealth; }
float Character::getMaxHealth() const { return maxHealth; }
bool Character::isFacingRight() const { return facingRight; }
bool Character::isGrounded() const { return positionY <= 120.0f; }
PlayerState Character::getState() const { return currentState; }
AABB Character::getHitBox() const { return hitbox.playerBox; }

bool Character::loadAttack(
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

void Character::renderDamageBox(const std::string &name)
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

void Character::performAttack(const std::string &name)
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

std::string Character::getCurrentAttackName() const { return currentAttackName; }

void Character::updateAttack(float deltaTime)
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

bool Character::isActiveAttack() const
{
    if (currentState != PlayerState::ATTACK || currentAttackName.empty()) return false;

    auto it = attacks.find(currentAttackName);
    if (it == attacks.end()) return false;

    const AttackData &data = it->second;
    int startupEnd = data.startupFrame;
    int activeEnd = data.activeFrame + startupEnd;

    return (frameCounter > startupEnd && frameCounter <= activeEnd);
}

bool Character::getActiveAttackHitbox(AABB &outBox) const
{
    if (!isActiveAttack()) return false;
    auto it = attacks.find(currentAttackName);
    if (it == attacks.end()) return false;

    outBox = it->second.hitBox.toWorld(positionX, positionY, facingRight);
    return true;
}

float Character::getCurrentAttackDamage() const
{
    auto it = attacks.find(currentAttackName);
    return (it != attacks.end()) ? it->second.damageAmount : 0.0f;
}

float Character::getCurrentAttackKnockback() const
{
    auto it = attacks.find(currentAttackName);
    return (it != attacks.end()) ? it->second.knockBackForce : 0.0f;
}

bool Character::getHasHit() const { return hasHit; }
void Character::markHit() { hasHit = true; }

void Character::collision(float leftLimit, float rightLimit)
{
    float playerRadius = 150.0f;
    float padding = 50.0f;

    if (positionX + playerRadius + padding >= rightLimit)
        positionX = rightLimit - playerRadius - padding;

    if (positionX - playerRadius - padding <= leftLimit)
        positionX = leftLimit + playerRadius + padding;
}