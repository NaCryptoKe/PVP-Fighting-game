#include "src/Character.hpp"

Character::Character()
    : currentAnim(nullptr),
      x(0.0f), y(0.0f),
      velocityX(0.0f), velocityY(0.0f),
      facingRight(true),
      currentState(CharacterState::IDLE),
      health(100), maxHealth(100) {}

bool Character::init(
    const char* idleFolder, int idleFrames,
    const char* jumpFolder, int jumpFrames
)
{
    // Idle loops forever; // jump plays once then we transition back out of it.
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

void Character::update(float deltaTime)
{
    if (!currentAnim)
        return; // init() wasn't called / failed - nothing to animate

    // --- Advance current animation ---
    currentAnim->update(deltaTime);

    // --- State/animation transitions ---
    // Right now the only transition rule is: jump plays once, then
    // fall back to idle. This is the same logic that used to live
    // directly in Game::update() - now it's centralized here so
    // future states (attack, hit-stun, etc.) all follow the same pattern.
    if (currentState == CharacterState::JUMPING && currentAnim->isFinished()) 
        setState(CharacterState::IDLE);

    // --- Apply movement ---
    x += velocityX * deltaTime;
    y += velocityY * deltaTime;

    // --- Push results into the sprite for rendering ---
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
    if (currentState == newState) {
        return; // already in this state, don't restart the animation
    }

    currentState = newState;

    switch (currentState) {
        case CharacterState::IDLE:
            currentAnim = &idleAnim;
            break;
        case CharacterState::JUMPING:
            currentAnim = &jumpAnim;
            break;
        // WALKING, CROUCHING, ATTACKING, BLOCKING, HIT_STUN:
        // no animations wired up yet - add cases here as those
        // Animation objects get added to Character.hpp
        default:
            currentAnim = &idleAnim;
            break;
    }

    currentAnim->reset(); // always start a newly-entered state's animation from frame 0
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