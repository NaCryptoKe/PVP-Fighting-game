#include "src/Character.hpp"

// ============================================================
// Constructor
// ============================================================

Character::Character()
    :   x(0.0f), y(0.0f),
        velocityX(0.0f), velocityY(0.0f),
        facingRight(true),
        currentState(CharacterState::IDLE),

        health(100), 
        maxHealth(100),

        groundY(0.0f),
        gravity(2200.0f),     // tune to taste
        jumpSpeed(900.0f),   // tune to taste
        walkSpeed(300.0f),

        hasHitThisAttack(false),
        hitStunTimer(0.0f),
        audio(std::make_unique<AudioManager>()) {}


// ========================================================
// Audio
// ========================================================
bool Character::addSfx(AudioState state, const char* filepath)
{
    if (!filepath) 
    {
        return false;
    }

    sfxs[state] = filepath;
    return true;
}

bool Character::setVolume(float volume)
{
    audio->set_master_volume(volume);
    return true;
}

bool Character::playSfx(AudioState state)
{
    // .find() instead of operator[]: a state with no registered sfx
    // (never passed to addSfx) should be a silent no-op, not a
    // silent insert of an empty-string entry into the map.
    auto it = sfxs.find(state);

    if (it == sfxs.end())
        return false;

    return audio->play_sound(it->second);
}

// Private functions
void Character::startAttack(AttackType type)
{
    currentAttack = type;

    setState(CharacterState::ATTACKING);
}

// ============================================================
// Initialization / Asset Loading
// ============================================================

/*
 * Intended way of using it is first
 * load the animations for the character
 * then initialize it
 */
bool Character::init ()
{
    currentState = CharacterState::IDLE;

    if (!audio->initialize()) {
        return false;
    }

    TextureData initialTex = animations[currentState].getCurrentTexture();
    if(initialTex.id == 0) return false;
    sprite = Sprite(initialTex);
    sprite.setFlip(facingRight);

    return true;
}

bool Character::loadIdleAnimation(
    const char* folder,
    int frames,
    float duration
)
{
    Animation idle;

    if (!idle.loadFromFiles(folder, frames, duration, true))
    {
        return false;
    }

    animations[CharacterState::IDLE] = idle;

    return true;
}

bool Character::loadWalkAnimation(
    const char* folder,
    int frames,
    float duration
)
{
    Animation walk;

    if (!walk.loadFromFiles(folder, frames, duration, true))
    {
        return false;
    }

    animations[CharacterState::WALKING] = walk;

    return true;
}

bool Character::loadJumpAnimation(
    const char* folder,
    int frames,
    float duration
)
{
    Animation jump;

    if (!jump.loadFromFiles(folder, frames, duration, false))
    {
        return false;
    }

    animations[CharacterState::JUMPING] = jump;

    return true;
}

bool Character::loadCrouchAnimation(
    const char* folder,
    int frames,
    float duration
)
{
    Animation crouch;

    if (!crouch.loadFromFiles(folder, frames, duration, true))
    {
        return false;
    }

    animations[CharacterState::CROUCHING] = crouch;

    return true;
}

bool Character::loadBlockAnimation(
    const char* folder,
    int frames,
    float duration
)
{
    Animation block;

    if (!block.loadFromFiles(folder, frames, duration, true))
    {
        return false;
    }

    animations[CharacterState::BLOCKING] = block;

    return true;
}

bool Character::loadHitStunAnimation(
    const char* folder,
    int frames,
    float duration
)
{
    Animation hitStun;

    if (!hitStun.loadFromFiles(folder, frames, duration, false))
    {
        return false;
    }

    animations[CharacterState::HIT_STUN] = hitStun;

    return true;
}

bool Character::loadKOAnimation(
    const char* folder,
    int frames,
    float duration
)
{
    Animation KO;

    if (!KO.loadFromFiles(folder, frames, duration, false))
    {
        return false;
    }

    animations[CharacterState::KO] = KO;

    return true;
}

bool Character::loadAttack(
    AttackType type,
    const char* folder, int frames, float duration,
    int activeStartFrame, int activeEndFrame,
    int damage,
    float hbOffsetX, float hbOffsetY, float hbWidth, float hbHeight
)
{
    Animation attack;

    if (!attack.loadFromFiles(folder, frames, duration, false))
    {
        return false;
    }

    AttackData attackData;

    attackData.anim             = attack;

    attackData.activeStartFrame = activeStartFrame;
    attackData.activeEndFrame   = activeEndFrame;
    attackData.damage           = damage;

    attackData.hitbox.offsetX   = hbOffsetX;
    attackData.hitbox.offsetY   = hbOffsetY;
    attackData.hitbox.width     = hbWidth;
    attackData.hitbox.height    = hbHeight;

    attackData.loaded = true;

    attacks[type] = attackData;

    return true;
}


// ============================================================
// Character Position / Hurtboxes
// ============================================================

void Character::setGroundY(float ground)
{
    groundY = ground;
    y = ground;
}

void Character::setHurtboxes(
    float standW,
    float standH,
    float crouchW,
    float crouchH
)
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


// ============================================================
// Input
// ============================================================

void Character::handleInput(const InputManager& input)
{
    // --------------------------------------------------------
    // No control during hit stun / KO
    // --------------------------------------------------------

    if (currentState == CharacterState::HIT_STUN ||
        currentState == CharacterState::KO)
        return;

    bool onGround = (y <= groundY);


    // --------------------------------------------------------
    // JUMP
    // --------------------------------------------------------

    if (input.isActionPressed(InputAction::JUMP) && onGround)
    {
        velocityY = jumpSpeed;
        velocityX = 0.0f;

        setState(CharacterState::JUMPING);

        return;
    }

    // --------------------------------------------------------
    // BLOCK
    // --------------------------------------------------------

    if (onGround && input.isActionHeld(InputAction::BLOCK))
    {
        velocityX = 0.0f;

        setState(CharacterState::BLOCKING);

        return;
    }

    // --------------------------------------------------------
    // CROUCH
    // --------------------------------------------------------

    if (onGround && input.isActionHeld(InputAction::CROUCH))
    {
        velocityX = 0.0f;

        setState(CharacterState::CROUCHING);

        return;
    }

    // --------------------------------------------------------
    // HORIZONTAL MOVEMENT
    // --------------------------------------------------------

    bool movingForward = input.isActionHeld(InputAction::FORWARD);

    bool movingBackward = input.isActionHeld(InputAction::BACKWARD);

    if (movingForward && !movingBackward)
    {
        velocityX = facingRight ? walkSpeed : -walkSpeed;

        if (onGround)
        {
            setState(CharacterState::WALKING);
        }
    }
    else if (movingBackward && !movingForward)
    {
        velocityX = facingRight ? -walkSpeed : walkSpeed;

        if (onGround)
        {
            setState(CharacterState::WALKING);
        }
    }
    else
    {
        velocityX = 0.0f;

        if (onGround)
        {
            setState(CharacterState::IDLE);
        }
    }

    // --------------------------------------------------------------------------------
    // Attacks
    // --------------------------------------------------------------------------------
    if (input.isActionPressed(InputAction::LIGHT_PUNCH)) startAttack(AttackType::LIGHT_PUNCH);
    else if (input.isActionPressed(InputAction::HARD_PUNCH)) startAttack(AttackType::HEAVY_PUNCH);
    else if (input.isActionPressed(InputAction::LIGHT_KICK)) startAttack(AttackType::LIGHT_KICK);
    else if (input.isActionPressed(InputAction::HARD_KICK)) startAttack(AttackType::HEAVY_KICK);
}

// ============================================================
// Facing
// ============================================================

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


// ============================================================
// Update / Physics / Animation
// ============================================================

void Character::update(float deltaTime)
{
    // --------------------------------------------------------
    // Jump physics
    // --------------------------------------------------------

    if (currentState == CharacterState::JUMPING)
    {
        velocityY -= gravity * deltaTime;
    }


    // --------------------------------------------------------
    // Hit stun
    // --------------------------------------------------------

    if (currentState == CharacterState::HIT_STUN)
    {
        hitStunTimer -= deltaTime;

        if (hitStunTimer <= 0.0f)
        {
            setState(CharacterState::IDLE);
        }
    }


    // --------------------------------------------------------
    // Apply velocity
    // --------------------------------------------------------

    x += velocityX * deltaTime;
    y += velocityY * deltaTime;


    // --------------------------------------------------------
    // Ground collision
    // --------------------------------------------------------

    if (y <= groundY)
    {
        y = groundY;
        velocityY = 0.0f;

        if (currentState == CharacterState::JUMPING)
        {
            setState(
                velocityX != 0.0f
                    ? CharacterState::WALKING
                    : CharacterState::IDLE
            );
        }
    }


    // --------------------------------------------------------
    // Get current animation
    // --------------------------------------------------------
    // Updating attack animations
    if (currentState == CharacterState::ATTACKING)
    {
        AttackData& attack = attacks.at(currentAttack);

        attack.anim.update(deltaTime);

        if (attack.anim.isFinished())
        {
            setState(CharacterState::IDLE);
        }
    }
    // Updating normal animations
    else
    {
        auto currentAnim = animations.find(currentState);

        if (currentAnim != animations.end())
        {
            currentAnim->second.update(deltaTime);

            sprite.setTexture(
                currentAnim->second.getCurrentTexture()
            );
        }
    }


    // --------------------------------------------------------
    // Sprite
    // --------------------------------------------------------

    sprite.setPosition(x, y);
    sprite.setFlip(!facingRight);
}


// ============================================================
// Rendering
// ============================================================

void Character::render()
{
    sprite.draw();
}


// ============================================================
// Character State
// ============================================================

void Character::setState(CharacterState newState)
{
    // Don't do anything if we're already in this state
    if (currentState == newState)
        return;

    currentState = newState;

    if (currentState == CharacterState::ATTACKING)
    {
        playSfx(AudioState::ATTACK);
        hasHitThisAttack = false;
        AttackData& attack = attacks.at(currentAttack);
        attack.anim.reset();
        sprite.setTexture(
            attack.anim.getCurrentTexture()
        );
        return;
    }

    // --------------------------------------------------------
    // Reset state animation
    // --------------------------------------------------------

    auto animation = animations.find(currentState);

    if (animation != animations.end())
    {
        animation->second.reset();

        sprite.setTexture(
            animation->second.getCurrentTexture()
        );
    }

    // --------------------------------------------------------
    // State-specific setup
    // --------------------------------------------------------

    if (currentState == CharacterState::HIT_STUN)
    {
        hitStunTimer = HIT_STUN_DURATION;
    }

    if (currentState == CharacterState::ATTACKING)
    {
        hasHitThisAttack = false;
    }

    switch(currentState)
    {
        case CharacterState::BLOCKING:
            playSfx(AudioState::BLOCK);
            break;
        case CharacterState::IDLE:
            playSfx(AudioState::IDLE);
            break;
        case CharacterState::JUMPING:
            playSfx(AudioState::JUMP);
            break;
        case CharacterState::KO:
            playSfx(AudioState::KO);
            break;
        default:
            break;
    }
}

// ============================================================
// Position / Facing / Scale
// ============================================================

void Character::setPosition(float px, float py)
{
    x = px;
    y = py;
}

void Character::setFacing(bool right)
{
    facingRight = right;
}

void Character::setScale(float scale)
{
    sprite.setScale(scale);
}


// ============================================================
// Combat - Active Hitbox
// ============================================================

bool Character::hasActiveHitbox() const
{
    if (currentState != CharacterState::ATTACKING)
        return false;

    if (hasHitThisAttack)
        return false; // already connected this swing - no double-hit

    const AttackData& atk = attacks.at(currentAttack);

    if (!atk.loaded)
        return false;

    int frame = atk.anim.getCurrentFrameIndex();

    return frame >= atk.activeStartFrame &&
           frame <= atk.activeEndFrame;
}

AABB Character::getActiveHitboxWorld() const
{
    const AttackData& atk = attacks.at(currentAttack);

    return atk.hitbox.toWorld(
        x,
        y,
        facingRight
    );
}

int Character::getActiveHitboxDamage() const
{
    return attacks.at(currentAttack).damage;
}


// ============================================================
// Combat - Hurtbox
// ============================================================

AABB Character::getHurtboxWorld() const
{
    const HitBox& hb =
        (currentState == CharacterState::CROUCHING)
            ? crouchingHurtbox
            : standingHurtbox;

    return hb.toWorld(
        x,
        y,
        facingRight
    );
}


// ============================================================
// Combat - Blocking / Hit Registration
// ============================================================

bool Character::isBlocking() const
{
    return currentState == CharacterState::BLOCKING;
}

void Character::registerHitLanded()
{
    hasHitThisAttack = true;
}


// ============================================================
// Combat - Apply Damage
// ============================================================

void Character::applyHit(int damage, bool wasBlocked)
{
    if (currentState == CharacterState::KO)
        return;

    int finalDamage =
        wasBlocked
            ? (damage / 10)
            : damage; // heavy chip reduction while blocking

    if (finalDamage < 1 && damage > 0)
        finalDamage = 1;

    health -= finalDamage;

    if (health < 0)
        health = 0;


    // --------------------------------------------------------
    // KO
    // --------------------------------------------------------

    if (health == 0)
    {
        setState(CharacterState::KO);
        return;
    }


    // --------------------------------------------------------
    // Hit stun
    // --------------------------------------------------------

    if (!wasBlocked)
    {
        velocityX = 0.0f;
        setState(CharacterState::HIT_STUN);
    }

    // Blocked hits stay in BLOCKING state -
    // no stun, just chip damage.
}


// ============================================================
// Getters
// ============================================================

float Character::getX() const
{
    return x;
}

float Character::getY() const
{
    return y;
}

int Character::getHealth() const
{
    return health;
}

int Character::getMaxHealth() const
{
    return maxHealth;
}

bool Character::isKO() const
{
    return currentState == CharacterState::KO;
}

bool Character::getFacingRight() const
{
    return facingRight;
}
