#include "entities/Character.h"

#include "GL/glut.h"
#include "render/Renderer.h"
#include <iostream>
#include <cmath>

Character::Character(float posX, float posY, float HP)
    :   positionX(posX), positionY(posY), velocityX(0.0f), velocityY(0.0f),
        maxHealth(HP), currentHealth(HP),
        facingRight(true), currentState(CharacterState::IDLE) {}

void Character::applyData(const CharacterData& data)
{
    maxHealth = data.maxHealth;
    currentHealth = data.maxHealth;
    walkSpeed = data.walkSpeed;
    jumpForce = data.jumpForce;
    setHitBox(data.hitBoxOffsetX, data.hitBoxOffsetY,
              data.hitBoxWidth, data.hitBoxHeight);

    for (const auto& atk : data.attacks)
    {
        loadAttack(atk.type, atk.startupFrame, atk.activeFrame,
                   atk.recoveryFrame, atk.damageAmount,
                   atk.hboffsetX, atk.hboffsetY, atk.width, atk.height,
                   atk.name, atk.knockBackForce, atk.blockable);
    }

    // Load every sprite animation from the roster data.
    // If a folder is missing, loadFromFiles fails gracefully and the
    // character still renders as the fallback colored quad.
    animations.clear();
    for (const auto& anim : data.animations)
    {
        Animation loaded;
        if (loaded.loadFromFiles(anim.folderPath.c_str(),
                                 anim.frameCount, anim.frameDuration, anim.looping))
        {
            animations[anim.stateName] = loaded;
        }
        else
        {
            std::cout << "Character: no animation for '" << anim.stateName
                      << "' (\"" << anim.folderPath << "\"), using fallback\n";
        }
    }

    // Give the sprite a default texture (the first IDLE frame) so the
    // character has a visible look before any state animation plays.
    auto idleIt = animations.find("IDLE");
    if (idleIt != animations.end())
        sprite.setTexture(idleIt->second.getCurrentTexture());
}

void Character::update(float deltaTime)
{
    float groundLevel = 120.0f;
    const float gravity = 7349.94f;

    // Ground friction — knockback (from hits or blocks) decays over time
    // so a character never slides forever. Skipped while actively walking
    // so holding a direction still moves at full walk speed.
    if (currentState != CharacterState::WALK)
    {
        const float friction = 8.0f;
        velocityX -= velocityX * friction * deltaTime;
        if (std::abs(velocityX) < 5.0f) velocityX = 0.0f;
    }

    if (positionY > groundLevel)
    {
        velocityY -= gravity * deltaTime;
    }

    positionX += velocityX * deltaTime;
    positionY += velocityY * deltaTime;

    if (positionY < groundLevel)
    {
        positionY = groundLevel;
        velocityY = 0.0f;
        if (currentState == CharacterState::JUMP) currentState = CharacterState::IDLE;
    }

    moveHitbox();
    updateAnimation(deltaTime);
}

void Character::render()
{
    // If sprite has a valid texture, render it; otherwise use fallback colored quad
    if (sprite.getTexture().id != 0)
    {
        sprite.setPosition(positionX, positionY);
        sprite.setFlip(facingRight);
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
                case CharacterState::IDLE:    glColor3f(0.0f, 1.0f, 0.0f); break;
                case CharacterState::WALK:    glColor3f(0.0f, 0.0f, 1.0f); break;
                case CharacterState::RUN:     glColor3f(1.0f, 0.0f, 0.0f); break;
                case CharacterState::JUMP:    glColor3f(1.0f, 1.0f, 0.0f); break;
                case CharacterState::ATTACK:  glColor3f(1.0f, 0.5f, 0.0f); break;
                case CharacterState::BLOCK:   glColor3f(0.5f, 0.5f, 0.5f); break;
                case CharacterState::HITSTUN: glColor3f(1.0f, 0.0f, 1.0f); break;
                case CharacterState::DEATH:   glColor3f(0.1f, 0.1f, 0.1f); break;
                case CharacterState::CROUCH:  glColor3f(0.55f, 0.27f, 0.07f); break;
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
        glVertex2f(hitbox.box.left, hitbox.box.top);
        glVertex2f(hitbox.box.left, hitbox.box.bottom);
        glVertex2f(hitbox.box.right, hitbox.box.bottom);
        glVertex2f(hitbox.box.right, hitbox.box.top);
    glEnd();
}

void Character::takeDamage(float damage)
{
    if (currentState == CharacterState::BLOCK) damage /= 4.0f;
    currentHealth -= damage;
    if (currentHealth <= 0.0f) currentHealth = 0.0f;
}
void Character::onHit(float damage, float knockback, bool pushRight, float hitstunTime)
{
    bool wasBlocking = (currentState == CharacterState::BLOCK);
    takeDamage(damage);

    if (currentHealth <= 0.0f)
    {
        currentState = CharacterState::DEATH;
        velocityX = 0.0f;
        return;
    }

    // Knockback logic
    float direction = pushRight ? 1.0f : -1.0f;

    if (wasBlocking)
    {
        velocityX += direction * knockback * 0.5f;
    }
    else
    {
        velocityX += direction * knockback;
        currentState = CharacterState::HITSTUN;
        hitstunTimer = hitstunTime;
    }
}

void Character::updateHitstun(float deltaTime)
{
    if (currentState != CharacterState::HITSTUN) return;

    hitstunTimer -= deltaTime;
    if (hitstunTimer <= 0.0f)
    {
        hitstunTimer = 0.0f;
        currentState = CharacterState::IDLE;
    }
}

void Character::moveFront()
{
    if (!canMove()) return;
    currentState = CharacterState::WALK;
    velocityX = facingRight ? walkSpeed : -walkSpeed;
}

void Character::moveBack()
{
    if (!canMove()) return;
    currentState = CharacterState::WALK;
    velocityX = facingRight ? -walkSpeed : walkSpeed;
}

void Character::stopX()
{
    if (currentState != CharacterState::WALK && currentState != CharacterState::RUN) return;
    velocityX = 0.0f;
    currentState = CharacterState::IDLE;
}

void Character::jump()
{
    if (!isGrounded() || (currentState == CharacterState::CROUCH)) return;

    velocityY = jumpForce;
    currentState = CharacterState::JUMP;
}

void Character::autoFace(float opponentX)
{
    if (!canAct()) return;
    if (currentState == CharacterState::ATTACK) return;

    facingRight = opponentX > positionX;
}

void Character::moveHitbox()
{
    hitbox.box = hitbox.toWorld(positionX, positionY, facingRight);
}

void Character::setBlocking(bool wantBlock)
{
    if (!canAct()) return;
    if (!isGrounded()) return;

    if (wantBlock && currentState != CharacterState::ATTACK && currentState != CharacterState::HITSTUN)
    {
        if (currentState != CharacterState::BLOCK)
        {
            currentState = CharacterState::BLOCK;
            velocityX = 0.0f;
        }
    }
    else if (!wantBlock && currentState == CharacterState::BLOCK)
    {
        currentState = CharacterState::IDLE;
    }
}

bool Character::isBlocking() const { return currentState == CharacterState::BLOCK; }
bool Character::canAct() const { return currentState != CharacterState::HITSTUN && currentState != CharacterState::DEATH && currentState != CharacterState::ATTACK; }
bool Character::canMove() const { return canAct() && currentState != CharacterState::CROUCH; }

bool Character::isDead() const { return currentHealth <= 0.0f; }

void Character::setCrouching(bool wantCrouch)
{
    if (!canAct()) return;
    if (!isGrounded()) return;

    if (wantCrouch)
    {
        if (currentState != CharacterState::ATTACK && currentState != CharacterState::HITSTUN && currentState != CharacterState::BLOCK)
        {
            currentState = CharacterState::CROUCH;
            velocityX = 0.0f;
        }
    }
    else if (currentState == CharacterState::CROUCH)
    {
        currentState = CharacterState::IDLE;
    }
}

void Character::resetForRound(float posX, float posY)
{
    positionX = posX;
    positionY = posY;
    velocityX = 0.0f;
    velocityY = 0.0f;
    currentHealth = maxHealth;
    facingRight = true;
    currentState = CharacterState::IDLE;
    currentAttackName = "";
    frameCounter = 0;
    frameAccumulator = 0.0f;
    hasHit = false;
}

void Character::setPositionX(float posX) { positionX = posX; }
void Character::setPositionY(float posY) { positionY = posY; }
void Character::setVelocityX(float velX) { velocityX = velX; }
void Character::setVelocityY(float velY) { velocityY = velY; }
void Character::setFacingRight(bool fr) { facingRight = fr; }
void Character::setState(CharacterState state) { currentState = state; }

void Character::setHitBox(float offsetX, float offsetY, float width, float height)
{
    hitbox.offsetX = offsetX;
    hitbox.offsetY = offsetY;
    hitbox.width = width;
    hitbox.height = height;
}

float Character::getPositionX() const { return positionX; }
float Character::getPositionY() const { return positionY; }
float Character::getVelocityX() const { return velocityX; }
float Character::getVelocityY() const { return velocityY; }
float Character::getHealth() const { return currentHealth; }
float Character::getMaxHealth() const { return maxHealth; }
bool Character::isFacingRight() const { return facingRight; }

bool Character::isGrounded() const { return positionY <= 120.0f; }
CharacterState Character::getState() const { return currentState; }
AABB Character::getHitBox() const { return hitbox.box; }
// ----------------------------------------------------------------
// Attack System
// ----------------------------------------------------------------

bool Character::loadAttack(
    AttackType type, int startupFrame, int activeFrame, int recoveryFrame,
    float damageAmount, float hboffsetX, float hboffsetY, float width, float height,
    const std::string &name, float knockBackForce, bool blockable)
{
    AttackData data;
    data.type = type;
    data.startupFrame = startupFrame;
    data.activeFrame = activeFrame;
    data.recoveryFrame = recoveryFrame;
    data.damageAmount = damageAmount;
    data.knockBackForce = knockBackForce;
    data.blockable = blockable;
    data.hitBox.offsetX = hboffsetX;
    data.hitBox.offsetY = hboffsetY;
    data.hitBox.width = width;
    data.hitBox.height = height;

    attacks[name] = data;
    return true;
}

void Character::renderDamageBox(const std::string &name)
{
    auto it = attacks.find(name);
    if (it == attacks.end()) return;

    AABB damageBox = it->second.hitBox.toWorld(positionX, positionY, facingRight);

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
    if (currentState == CharacterState::BLOCK) return;
    if (!isGrounded()) return;

    auto it = attacks.find(name);
    if (it == attacks.end()) return;

    currentAttackName = name;
    currentState = CharacterState::ATTACK;
    frameCounter = 0;
    frameAccumulator = 0.0f;
    hasHit = false;
    velocityX = 0.0f;
}

std::string Character::getCurrentAttackName() const { return currentAttackName; }

void Character::updateAttack(float deltaTime)
{
    if (currentState != CharacterState::ATTACK || currentAttackName.empty()) return;

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
        currentState = CharacterState::IDLE;
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
void Character::updateAnimation(float deltaTime)
{
    // Attack animations are advanced by updateAttack() already
    if (currentState == CharacterState::ATTACK) return;

    // Map the current state to its animation name (must match roster keys)
    std::string animName;
    switch (currentState)
    {
        case CharacterState::ATTACK:  return; // handled by updateAttack()
        case CharacterState::IDLE:    animName = "IDLE";   break;
        case CharacterState::WALK:    animName = "WALK";    break;
        case CharacterState::RUN:     animName = "RUN";     break;
        case CharacterState::JUMP:    animName = "JUMP";    break;
        case CharacterState::BLOCK:   animName = "BLOCK";   break;
        case CharacterState::HITSTUN: animName = "HITSTUN"; break;
        case CharacterState::DEATH:   animName = "DEATH";   break;
        case CharacterState::CROUCH:  animName = "CROUCH";  break;
    }

    auto it = animations.find(animName);
    if (it == animations.end()) return;

    it->second.update(deltaTime);
    sprite.setTexture(it->second.getCurrentTexture());
}

bool Character::isActiveAttack() const
{
    if (currentState != CharacterState::ATTACK || currentAttackName.empty()) return false;

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
    float characterRadius = 150.0f;
    float padding = 50.0f;

    if (positionX + characterRadius + padding >= rightLimit)
        positionX = rightLimit - characterRadius - padding;

    if (positionX - characterRadius - padding <= leftLimit)
        positionX = leftLimit + characterRadius + padding;
}