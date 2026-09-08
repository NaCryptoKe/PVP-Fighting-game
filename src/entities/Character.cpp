#include "entities/Character.h"

#include "GL/glut.h"
#include "render/Renderer.h"
#include <algorithm>
#include <cmath>
#include <iostream>

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
                   atk.name, atk.knockBackForce, atk.blockable, atk.hitstunTime);
    }

    // Match the roster's sprite scale so the frames line up with the hitbox
    sprite.setScale(data.spriteScale);

    // Load every sprite animation from the roster data.
    // If a folder is missing, loadFromFiles fails gracefully and the
    // character still renders as the fallback colored quad.
    animations.clear();
    currentAnimName.clear();
    for (const auto& anim : data.animations)
    {
        Animation loaded;
        if (loaded.loadFromFiles(anim.folderPath.c_str(),
                                 anim.frameCount, anim.frameDuration, anim.looping,
                                 anim.startFrame))
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
    {
        idleIt->second.reset();
        currentAnimName = "IDLE";
        sprite.setTexture(idleIt->second.getCurrentTexture());
    }
}

void Character::update(float deltaTime)
{
    float groundLevel = 120.0f;
    const float gravity = 7349.94f;

    // Timers
    if (fireballCooldown > 0.0f) fireballCooldown -= deltaTime;
    if (blockFlashTimer  > 0.0f) blockFlashTimer  -= deltaTime;

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
        // Land — an air attack in progress finishes before returning to IDLE
        if (currentState == CharacterState::JUMP) currentState = CharacterState::IDLE;
    }

    // Crouching lowers the hurtbox (standing 300 tall, crouching 180 tall)
    hitbox.height = (crouching && isGrounded()) ? 180.0f : 300.0f;

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
                case CharacterState::IDLE:         glColor3f(0.0f, 1.0f, 0.0f); break;
                case CharacterState::WALK:         glColor3f(0.0f, 0.0f, 1.0f); break;
                case CharacterState::RUN:          glColor3f(1.0f, 0.0f, 0.0f); break;
                case CharacterState::JUMP:         glColor3f(1.0f, 1.0f, 0.0f); break;
                case CharacterState::ATTACK:       glColor3f(1.0f, 0.5f, 0.0f); break;
                case CharacterState::BLOCK:        glColor3f(0.5f, 0.5f, 0.5f); break;
                case CharacterState::CROUCH_BLOCK: glColor3f(0.4f, 0.4f, 0.55f); break;
                case CharacterState::HITSTUN:      glColor3f(1.0f, 0.0f, 1.0f); break;
                case CharacterState::DEATH:        glColor3f(0.1f, 0.1f, 0.1f); break;
                case CharacterState::CROUCH:       glColor3f(0.55f, 0.27f, 0.07f); break;
                case CharacterState::VICTORY:      glColor3f(1.0f, 0.85f, 0.0f); break;
            }
            glVertex2f(-length / 2.0f, 0.0f);
            glVertex2f( length / 2.0f, 0.0f);
            glVertex2f( length / 2.0f, length);
            glVertex2f(-length / 2.0f, length);
        glEnd();

        glPopMatrix();
    }
}

// Semi-transparent hurtbox (debug view, toggled by holding 0)
void Character::renderHitBox()
{
    Renderer::drawQuad(hitbox.box.left, hitbox.box.bottom,
                       hitbox.box.right - hitbox.box.left,
                       hitbox.box.top - hitbox.box.bottom,
                       1.0f, 1.0f, 1.0f, 0.55f);
}

// Semi-transparent damage box for the active attack frames
void Character::renderDamageBox(const std::string &name)
{
    auto it = attacks.find(name);
    if (it == attacks.end()) return;

    AABB damageBox = it->second.hitBox.toWorld(positionX, positionY, facingRight);

    Renderer::drawQuad(damageBox.left, damageBox.bottom,
                       damageBox.right - damageBox.left,
                       damageBox.top - damageBox.bottom,
                       0.0f, 1.0f, 1.0f, 0.55f);
}

void Character::takeDamage(float damage)
{
    currentHealth = std::max(currentHealth - damage, 0.0f);
}

void Character::onHit(const HitImpact& impact)
{
    bool blocked = isBlocking() && impact.blockable;

    takeDamage(blocked ? impact.damage / 4.0f : impact.damage);

    if (currentHealth <= 0.0f)
    {
        currentState = CharacterState::DEATH;
        hitstunTimer = 0.0f;
        blockFlashTimer = 0.0f;
        velocityX = 0.0f;
        return;
    }

    float direction = impact.pushRight ? 1.0f : -1.0f;

    if (blocked)
    {
        // Chip damage, half knockback, brief guard flash
        velocityX += direction * impact.knockback * 0.5f;
        blockFlashTimer = 0.25f;
        return;
    }

    velocityX += direction * impact.knockback;
    currentState = CharacterState::HITSTUN;
    hitstunTimer = impact.hitstunTime;

    // Reaction clip: thrown override > crouching > heavy blow > standard
    if (!impact.animOverride.empty())
        hitstunAnimOverride = impact.animOverride;
    else if (crouching)
        hitstunAnimOverride = "CROUCH_STUN";
    else if (impact.hitstunTime > 0.35f)
        hitstunAnimOverride = "HIT_BLOW";
    else
        hitstunAnimOverride.clear();
}

void Character::updateHitstun(float deltaTime)
{
    if (currentState != CharacterState::HITSTUN) return;

    hitstunTimer -= deltaTime;
    if (hitstunTimer <= 0.0f)
    {
        hitstunTimer = 0.0f;
        hitstunAnimOverride.clear();
        crouching = false;   // stand back up after the reaction
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
    if (!isGrounded()) return;
    if (currentState == CharacterState::CROUCH || currentState == CharacterState::CROUCH_BLOCK) return;

    velocityY = jumpForce;
    currentState = CharacterState::JUMP;
}

void Character::autoFace(float opponentX)
{
    // Facing tracks the opponent while idle, walking, guarding (even in
    // blockstun) and jumping — but not during attacks or reactions
    if (currentState == CharacterState::ATTACK ||
        currentState == CharacterState::HITSTUN ||
        currentState == CharacterState::DEATH ||
        currentState == CharacterState::VICTORY) return;
    facingRight = opponentX > positionX;
}

void Character::moveHitbox()
{
    hitbox.box = hitbox.toWorld(positionX, positionY, facingRight);
}

// Replaces the old setBlocking/setCrouching pair: derives the guard state from
// the two held inputs (block and crouch combine into CROUCH_BLOCK).
void Character::updateGuard(bool wantBlock, bool wantCrouch)
{
    crouching = wantCrouch && isGrounded();

    if (!canAct() || !isGrounded()) return;

    if (wantBlock && wantCrouch)
    {
        if (currentState != CharacterState::CROUCH_BLOCK)
        {
            currentState = CharacterState::CROUCH_BLOCK;
            velocityX = 0.0f;
        }
    }
    else if (wantBlock)
    {
        if (currentState != CharacterState::BLOCK)
        {
            currentState = CharacterState::BLOCK;
            velocityX = 0.0f;
        }
    }
    else if (wantCrouch)
    {
        if (currentState != CharacterState::CROUCH)
        {
            currentState = CharacterState::CROUCH;
            velocityX = 0.0f;
        }
    }
    else if (currentState == CharacterState::BLOCK ||
             currentState == CharacterState::CROUCH_BLOCK ||
             currentState == CharacterState::CROUCH)
    {
        currentState = CharacterState::IDLE;
    }
}

void Character::setVictory(bool matchWon)
{
    if (currentHealth <= 0.0f) return;   // a KO'd fighter never celebrates

    victoryAnimKey = matchWon ? "CELEBRATION" : "VICTORY";
    currentState = CharacterState::VICTORY;
    velocityX = 0.0f;
}

bool Character::isBlocking() const
{
    return currentState == CharacterState::BLOCK || currentState == CharacterState::CROUCH_BLOCK;
}

bool Character::canAct() const
{
    // blockFlashTimer doubles as blockstun: while it runs the guard is
    // locked (no attacking, no walking, no guard switching)
    return currentState != CharacterState::HITSTUN &&
           currentState != CharacterState::DEATH &&
           currentState != CharacterState::ATTACK &&
           currentState != CharacterState::VICTORY &&
           blockFlashTimer <= 0.0f;
}

bool Character::canMove() const
{
    return canAct() &&
           currentState != CharacterState::CROUCH &&
           currentState != CharacterState::BLOCK &&
           currentState != CharacterState::CROUCH_BLOCK;
}

bool Character::isDead() const { return currentHealth <= 0.0f; }

void Character::resetForRound(float posX, float posY)
{
    positionX = posX;
    positionY = posY;
    velocityX = 0.0f;
    velocityY = 0.0f;
    currentHealth = maxHealth;
    facingRight = true;
    currentState = CharacterState::IDLE;
    currentAttackName.clear();
    currentAttackAnimName.clear();
    currentAnimName.clear();
    frameCounter = 0;
    frameAccumulator = 0.0f;
    hasHit = false;
    crouching = false;
    hitstunTimer = 0.0f;
    hitstunAnimOverride.clear();
    blockFlashTimer = 0.0f;
    fireballCooldown = 0.0f;
    projectileSpawned = false;
    victoryAnimKey = "VICTORY";
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
    const std::string &name, float knockBackForce, bool blockable, float hitstunTime)
{
    AttackData data;
    data.type = type;
    data.startupFrame = startupFrame;
    data.activeFrame = activeFrame;
    data.recoveryFrame = recoveryFrame;
    data.damageAmount = damageAmount;
    data.knockBackForce = knockBackForce;
    data.hitstunTime = hitstunTime;
    data.blockable = blockable;
    data.hitBox.offsetX = hboffsetX;
    data.hitBox.offsetY = hboffsetY;
    data.hitBox.width = width;
    data.hitBox.height = height;

    attacks[name] = data;
    return true;
}

void Character::performAttack(const std::string &name)
{
    if (!canAct()) return;
    if (isBlocking()) return;

    // Fireballs and throws only from a standing grounded stance
    if (name == "FIREBALL" && (fireballCooldown > 0.0f || !isGrounded())) return;
    if (name == "THROW" && (crouching || !isGrounded())) return;

    auto it = attacks.find(name);
    if (it == attacks.end()) return;

    // Air attacks start from a jump; grounded attacks from the ground
    if (!isGrounded() && currentState != CharacterState::JUMP) return;

    // Resolve the animation variant: crouching and airborne attacks use their
    // own CROUCH_*/JUMP_* clips and fall back to the standing one.
    currentAttackAnimName = name;
    if (crouching && currentState == CharacterState::CROUCH)
    {
        std::string variant = "CROUCH_" + name;
        if (animations.count(variant)) currentAttackAnimName = variant;
    }
    else if (!isGrounded())
    {
        std::string variant = "JUMP_" + name;
        if (animations.count(variant)) currentAttackAnimName = variant;
    }

    currentAttackName = name;
    currentState = CharacterState::ATTACK;
    frameCounter = 0;
    frameAccumulator = 0.0f;
    hasHit = false;
    projectileSpawned = false;
    if (isGrounded()) velocityX = 0.0f;   // air attacks keep their momentum
    if (name == "FIREBALL") fireballCooldown = 1.0f;
}

// Returns true exactly once per fireball cast, when the startup frames finish
bool Character::consumeProjectileSpawn()
{
    if (currentAttackName != "FIREBALL" || currentState != CharacterState::ATTACK || projectileSpawned)
        return false;

    auto it = attacks.find("FIREBALL");
    if (it == attacks.end()) return false;

    if (frameCounter <= it->second.startupFrame) return false;

    projectileSpawned = true;
    return true;
}

std::string Character::getCurrentAttackName() const { return currentAttackName; }

void Character::updateAttack(float deltaTime)
{
    if (currentState != CharacterState::ATTACK || currentAttackName.empty()) return;

    auto dataIt = attacks.find(currentAttackName);
    if (dataIt == attacks.end()) return;
    const AttackData &data = dataIt->second;

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
        // Attack over: air attacks hand control back to the jump,
        // grounded ones to idle (the guard state re-applies next frame).
        currentState = isGrounded() ? CharacterState::IDLE : CharacterState::JUMP;
        currentAttackName.clear();
        currentAttackAnimName.clear();
        frameCounter = 0;
        frameAccumulator = 0.0f;
        hasHit = false;
    }

    // Update animation if it exists for this attack state
    playAnimation(currentAttackAnimName, deltaTime);
}

void Character::updateAnimation(float deltaTime)
{
    // Attack animations are advanced by updateAttack() already
    if (currentState == CharacterState::ATTACK) return;

    // Map the current state to its animation name (must match roster keys)
    std::string animName;
    switch (currentState)
    {
        case CharacterState::ATTACK: return; // handled by updateAttack()
        case CharacterState::IDLE:   animName = "IDLE"; break;
        case CharacterState::WALK:
            // Separate walk cycles for moving towards vs away from the opponent
            animName = isMovingForward() ? "WALK_FORWARD" : "WALK_BACKWARD";
            break;
        case CharacterState::RUN:    animName = "RUN"; break;
        case CharacterState::JUMP:
            animName = isMovingForward() ? "JUMP_FORWARD" : "JUMP";
            break;
        case CharacterState::BLOCK:
            // Brief flash when a hit is absorbed by the guard
            animName = (blockFlashTimer > 0.0f) ? "BLOCK_HIT" : "BLOCK";
            break;
        case CharacterState::CROUCH_BLOCK:
        {
            // Play the stand->crouch-block transition once, then hold the loop
            auto transitionIt = animations.find("BLOCK_TO_CROUCH_BLOCK");
            bool playTransition = transitionIt != animations.end() &&
                                  currentAnimName != "CROUCH_BLOCK" &&
                                  (currentAnimName != "BLOCK_TO_CROUCH_BLOCK" ||
                                   !transitionIt->second.isFinished());
            animName = playTransition ? "BLOCK_TO_CROUCH_BLOCK" : "CROUCH_BLOCK";
            break;
        }
        case CharacterState::CROUCH:  animName = "CROUCH"; break;
        case CharacterState::HITSTUN:
            animName = !hitstunAnimOverride.empty() ? hitstunAnimOverride
                     : (crouching ? "CROUCH_STUN" : "HITSTUN");
            break;
        case CharacterState::DEATH:
        {
            // Fall (KO) then rest lying on the ground (KO_idle)
            auto deathIt = animations.find("DEATH");
            auto restIt  = animations.find("DEATH_IDLE");
            bool playRest = deathIt != animations.end() && deathIt->second.isFinished() &&
                            restIt != animations.end();
            animName = playRest ? "DEATH_IDLE" : "DEATH";
            break;
        }
        case CharacterState::VICTORY: animName = victoryAnimKey; break;
    }

    playAnimation(animName, deltaTime);
}

// Advances the named animation and feeds the current frame to the sprite.
// Switching animations restarts the new one from frame 0 so one-shot clips
// (punches, hit reactions, KO) always replay from the beginning.
void Character::playAnimation(const std::string& name, float deltaTime)
{
    if (name != currentAnimName)
    {
        currentAnimName = name;
        auto resetIt = animations.find(name);
        if (resetIt != animations.end()) resetIt->second.reset();
    }

    auto it = animations.find(name);
    if (it == animations.end()) return;

    it->second.update(deltaTime);
    sprite.setTexture(it->second.getCurrentTexture());
}

// Forward means the velocity points in the direction the character faces
bool Character::isMovingForward() const
{
    if (std::abs(velocityX) < 1.0f) return facingRight;
    return (velocityX > 0.0f) == facingRight;
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

float Character::getCurrentAttackHitstun() const
{
    auto it = attacks.find(currentAttackName);
    return (it != attacks.end()) ? it->second.hitstunTime : 0.3f;
}

bool Character::isCurrentAttackBlockable() const
{
    auto it = attacks.find(currentAttackName);
    return (it != attacks.end()) ? it->second.blockable : true;
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
