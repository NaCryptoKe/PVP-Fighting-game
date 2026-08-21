#include "src/Character.hpp"

Character::Character()
    : currentAnim(nullptr),
      currentAttack(AttackType::LIGHT_PUNCH),
      hasHitThisAttack(false),
      x(0.0f), y(0.0f),
      velocityX(0.0f), velocityY(0.0f),
      facingRight(true),
      currentState(CharacterState::IDLE),
      groundY(0.0f),
      gravity(2200.0f),    // tune to taste
      jumpSpeed(1500.0f),   // tune to taste
      walkSpeed(300.0f),
      hitStunTimer(0.0f),
      health(100), maxHealth(100)
{
    setHurtboxes(80.0f, 180.0f, 90.0f, 110.0f); // sane defaults; override via setHurtboxes()
}

bool Character::init
(
    const char* idleFolder, int idleFrames,
    const char* jumpFolder, int jumpFrames
)
{
    bool idleOk = idleAnim.loadFromFiles(idleFolder, idleFrames, 0.12f, true);
    bool jumpOk = jumpAnim.loadFromFiles(jumpFolder, jumpFrames, 0.16f, false);

    if (!idleOk || !jumpOk) return false;

    currentAnim = &idleAnim;
    currentState = CharacterState::IDLE;

    TextureData initialTex = currentAnim->getCurrentTexture();
    sprite = Sprite(initialTex);
    sprite.setFlip(facingRight);

    return true;
}

bool Character::loadWalkAnimation(const char* folder, int frames)
{
    return walkAnim.loadFromFiles(folder, frames, 0.10f, true);
}

bool Character::loadCrouchAnimation(const char* folder, int frames)
{
    return crouchAnim.loadFromFiles(folder, frames, 0.10f, true);
}

bool Character::loadBlockAnimation(const char* folder, int frames)
{
    return blockAnim.loadFromFiles(folder, frames, 0.10f, true);
}

bool Character::loadHitAnimation(const char* folder, int frames)
{
    return hitAnim.loadFromFiles(folder, frames, 0.08f, false);
}

bool Character::loadAttack(
    AttackType type,
    const char* folder, int frameCount, float frameDuration,
    int activeStartFrame, int activeEndFrame,
    int damage,
    float hbOffsetX, float hbOffsetY, float hbWidth, float hbHeight
)
{
    AttackData& atk = attacks[(int)type];

    bool ok = atk.anim.loadFromFiles(folder, frameCount, frameDuration, false);
    if (!ok) return false;

    atk.activeStartFrame = activeStartFrame;
    atk.activeEndFrame   = activeEndFrame;
    atk.damage            = damage;
    atk.hitbox.offsetX    = hbOffsetX;
    atk.hitbox.offsetY    = hbOffsetY;
    atk.hitbox.width      = hbWidth;
    atk.hitbox.height     = hbHeight;
    atk.loaded             = true;

    return true;
}

void Character::setGroundY(float ground)
{
    groundY = ground;
    y = ground;
}

void Character::setHurtboxes(float standW, float standH, float crouchW, float crouchH)
{
    standingHurtbox.offsetX = -standW / 2.0f;
    standingHurtbox.offsetY = 0.0f;
    standingHurtbox.width   = standW;
    standingHurtbox.height  = standH;

    crouchingHurtbox.offsetX = -crouchW / 2.0f;
    crouchingHurtbox.offsetY = 0.0f;
    crouchingHurtbox.width   = crouchW;
    crouchingHurtbox.height  = crouchH;
}

void Character::handleInput(const InputManager& input)
{
    // No player control while stunned or knocked out.
    if (currentState == CharacterState::HIT_STUN || currentState == CharacterState::KO)
        return;

    // An attack is committed once started - ignore everything else
    // until update() sees the animation finish.
    if (currentState == CharacterState::ATTACKING)
        return;

    bool onGround = (currentState != CharacterState::JUMPING);

    // --- Attack triggers (grounded only) ---
    if (onGround)
    {
        AttackType triggered = AttackType::LIGHT_PUNCH;
        bool anyTriggered = false;

        if      (input.isActionPressed(InputAction::CROSS))    { triggered = AttackType::LIGHT_PUNCH; anyTriggered = true; }
        else if (input.isActionPressed(InputAction::CIRCLE))   { triggered = AttackType::HEAVY_PUNCH; anyTriggered = true; }
        else if (input.isActionPressed(InputAction::SQUARE))   { triggered = AttackType::LIGHT_KICK;  anyTriggered = true; }
        else if (input.isActionPressed(InputAction::TRIANGLE)) { triggered = AttackType::HEAVY_KICK;  anyTriggered = true; }

        if (anyTriggered && attacks[(int)triggered].loaded)
        {
            currentAttack = triggered;
            hasHitThisAttack = false;
            velocityX = 0.0f;
            setState(CharacterState::ATTACKING);
            return;
        }
    }

    // --- Jump ---
    if (input.isActionPressed(InputAction::JUMP) && onGround &&
        currentState != CharacterState::BLOCKING &&
        currentState != CharacterState::CROUCHING)
    {
        velocityY = jumpSpeed;
        setState(CharacterState::JUMPING);
        return;
    }

    // --- Block (grounded only) ---
    if (onGround && input.isActionHeld(InputAction::BLOCK))
    {
        velocityX = 0.0f;
        setState(CharacterState::BLOCKING);
        return;
    }

    // --- Crouch (grounded only) ---
    if (onGround && input.isActionHeld(InputAction::CROUCH))
    {
        velocityX = 0.0f;
        setState(CharacterState::CROUCHING);
        return;
    }

    // --- Horizontal movement (grounded or airborne - basic air control) ---
    bool movingForward  = input.isActionHeld(InputAction::FORWARD);
    bool movingBackward = input.isActionHeld(InputAction::BACKWARD);

    if (movingForward && !movingBackward)
        velocityX = facingRight ? walkSpeed : -walkSpeed;
    else if (movingBackward && !movingForward)
        velocityX = facingRight ? -walkSpeed : walkSpeed;
    else
        velocityX = 0.0f;

    if (onGround)
        setState(velocityX != 0.0f ? CharacterState::WALKING : CharacterState::IDLE);
}

void Character::faceToward(float opponentX)
{
    // Don't flip mid-attack/hitstun/KO - would desync the hitbox
    // position and look glitchy.
    if (currentState == CharacterState::ATTACKING ||
        currentState == CharacterState::HIT_STUN ||
        currentState == CharacterState::KO)
        return;

    facingRight = (opponentX >= x);
}

void Character::update(float deltaTime)
{
    if (!currentAnim)
        return;

    currentAnim->update(deltaTime);

    if (currentState == CharacterState::JUMPING)
        velocityY -= gravity * deltaTime;

    if (currentState == CharacterState::HIT_STUN)
    {
        hitStunTimer -= deltaTime;
        if (hitStunTimer <= 0.0f)
            setState(CharacterState::IDLE);
    }

    if (currentState == CharacterState::ATTACKING && currentAnim->isFinished())
        setState(CharacterState::IDLE);

    x += velocityX * deltaTime;
    y += velocityY * deltaTime;

    if (y <= groundY)
    {
        y = groundY;
        velocityY = 0.0f;
        if (currentState == CharacterState::JUMPING)
            setState(velocityX != 0.0f ? CharacterState::WALKING : CharacterState::IDLE);
    }

    sprite.setTexture(currentAnim->getCurrentTexture());
    sprite.setPosition(x, y);
    sprite.setFlip(!facingRight);
}

void Character::render()
{
    sprite.draw();
}

void Character::setState(CharacterState newState)
{
    if (currentState == newState)
        return;

    currentState = newState;

    switch (currentState)
    {
        case CharacterState::IDLE:
            currentAnim = &idleAnim;
            break;
        case CharacterState::WALKING:
            currentAnim = (walkAnim.getFrameCount() > 0) ? &walkAnim : &idleAnim;
            break;
        case CharacterState::CROUCHING:
            currentAnim = (crouchAnim.getFrameCount() > 0) ? &crouchAnim : &idleAnim;
            break;
        case CharacterState::JUMPING:
            currentAnim = &jumpAnim;
            break;
        case CharacterState::BLOCKING:
            currentAnim = (blockAnim.getFrameCount() > 0) ? &blockAnim : &idleAnim;
            break;
        case CharacterState::ATTACKING:
            currentAnim = &attacks[(int)currentAttack].anim;
            break;
        case CharacterState::HIT_STUN:
            hitStunTimer = HIT_STUN_DURATION;
            currentAnim = (hitAnim.getFrameCount() > 0) ? &hitAnim : &idleAnim;
            break;
        case CharacterState::KO:
            currentAnim = (hitAnim.getFrameCount() > 0) ? &hitAnim : &idleAnim;
            break;
    }

    currentAnim->reset();
}

void Character::setPosition(float px, float py)
{
    x = px;
    y = py;
}

void Character::setFacing(bool right)
{
    facingRight = right;
}

void Character::setScale(int scale)
{
    sprite.setScale(scale);
}

bool Character::hasActiveHitbox() const
{
    if (currentState != CharacterState::ATTACKING) return false;
    if (hasHitThisAttack) return false; // already connected this swing - no double-hit

    const AttackData& atk = attacks[(int)currentAttack];
    if (!atk.loaded) return false;

    int frame = atk.anim.getCurrentFrameIndex();
    return frame >= atk.activeStartFrame && frame <= atk.activeEndFrame;
}

AABB Character::getActiveHitboxWorld() const
{
    const AttackData& atk = attacks[(int)currentAttack];
    return atk.hitbox.toWorld(x, y, facingRight);
}

int Character::getActiveHitboxDamage() const
{
    return attacks[(int)currentAttack].damage;
}

AABB Character::getHurtboxWorld() const
{
    const HitBox& hb = (currentState == CharacterState::CROUCHING)
        ? crouchingHurtbox : standingHurtbox;
    return hb.toWorld(x, y, facingRight);
}

bool Character::isBlocking() const
{
    return currentState == CharacterState::BLOCKING;
}

void Character::registerHitLanded()
{
    hasHitThisAttack = true;
}

void Character::applyHit(int damage, bool wasBlocked)
{
    if (currentState == CharacterState::KO) return;

    int finalDamage = wasBlocked ? (damage / 10) : damage; // heavy chip reduction while blocking
    if (finalDamage < 1 && damage > 0) finalDamage = 1;

    health -= finalDamage;
    if (health < 0) health = 0;

    if (health == 0)
    {
        setState(CharacterState::KO);
        return;
    }

    if (!wasBlocked)
    {
        velocityX = 0.0f;
        setState(CharacterState::HIT_STUN);
    }
    // Blocked hits stay in BLOCKING state - no stun, just chip damage.
}

float Character::getX() const { return x; }
float Character::getY() const { return y; }
int Character::getHealth() const { return health; }
int Character::getMaxHealth() const { return maxHealth; }
bool Character::isKO() const { return currentState == CharacterState::KO; }
bool Character::getFacingRight() const { return facingRight; }
