#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "src/Sprite.hpp"
#include "src/Animation.hpp"
#include "src/Input.hpp"
#include "src/HitBox.hpp"

#include <unordered_map>

enum class CharacterState
{
    IDLE,
    WALKING,
    CROUCHING,
    JUMPING,
    ATTACKING,
    BLOCKING,
    HIT_STUN,
    KO
};

enum class AttackType
{
    LIGHT_PUNCH,  // CROSS
    HEAVY_PUNCH,  // CIRCLE
    LIGHT_KICK,   // SQUARE
    HEAVY_KICK    // TRIANGLE
};

struct AttackData
{
    Animation anim;
    bool loaded = false;

    int activeStartFrame = 0; // first frame (inclusive) hitbox is live
    int activeEndFrame    = 0; // last frame (inclusive) hitbox is live

    int damage = 0;
    HitBox hitbox; // authored in facing-right local space
};

class Character
{
private:
    // -----------------------------------------
    // Character position and info
    // -----------------------------------------
    float x;
    float y;
    float velocityX;
    float velocityY;
    bool facingRight;
    CharacterState currentState;

private:
    // -----------------------------------------
    // Character health
    // -----------------------------------------
    int health;
    int maxHealth;

private:
    // -----------------------------------------
    // Character movement
    // -----------------------------------------
    float groundY;
    float gravity;
    float jumpSpeed;
    float walkSpeed;

private:
    // -----------------------------------------
    // Character hurtboxes/hitbox et.c
    // -----------------------------------------
    HitBox standingHurtbox;     // Vulnerable areas to be hit
    HitBox crouchingHurtbox;

    HitBox hitBox;
    HitBox blockBox;

private:
    // -----------------------------------------
    // Character rendering
    // -----------------------------------------
    // The image of the character
    Sprite sprite;

private:
    // -----------------------------------------
    // Character animations
    // -----------------------------------------
    std::unordered_map<CharacterState, Animation> animations;
    std::unordered_map<AttackType, AttackData> attacks;

private:
    // -----------------------------------------
    // Character combat
    // -----------------------------------------
    AttackType currentAttack;
    bool hasHitThisAttack;  // Has the attack connected with the opponent

    float hitStunTimer; // The amount of time left for the stun animation to leave
    static constexpr float HIT_STUN_DURATION = 0.35f;

public:
    // -----------------------------------------
    // Constructor
    // -----------------------------------------
    Character();

public:
    // -----------------------------------------
    // Initialization
    // -----------------------------------------
    bool init();

public:
    // -----------------------------------------
    // Animation loading
    // -----------------------------------------
    bool loadIdleAnimation(const char* folder, int frames, float duration);
    bool loadWalkAnimation(const char* folder, int frames, float duration);
    bool loadJumpAnimation(const char* folder, int frames, float duration);
    bool loadCrouchAnimation(const char* folder, int frames, float duration);
    bool loadBlockAnimation(const char* folder, int frames, float duration);
    bool loadHitStunAnimation(const char* folder, int frames, float duration);
    bool loadKOAnimation(const char* folder, int frames, float duration);

    bool loadAttack(
        AttackType type,
        const char* folder, int frames, float duration,
        int activeStartFrame, int activeEndFrame,
        int damage,
        float hbOffsetX, float hbOffsetY, float hbWidth, float hbHeight
    );

public:
    // -----------------------------------------
    // Movement / Hurtbox setup
    // -----------------------------------------
    void setGroundY(float ground);
    void setHurtboxes(
        float standW,
        float standH,
        float crouchW,
        float crouchH
    );

public:
    // -----------------------------------------
    // Input / Facing
    // -----------------------------------------
    void handleInput(const InputManager& input);

    void faceToward(float opponentX);
    // auto-face; skipped mid-attack/hitstun/KO

public:
    // -----------------------------------------
    // Update / Rendering
    // -----------------------------------------
    void update(float deltaTime);

    void render();

public:
    // -----------------------------------------
    // Character state / position
    // -----------------------------------------
    void setState(CharacterState newState);
    void setPosition(float px, float py);
    void setFacing(bool right);

    void setScale(float scale);

public:
    // -----------------------------------------
    // Combat queries
    // Used by Game for hit detection
    // -----------------------------------------
    bool hasActiveHitbox() const;
    AABB getActiveHitboxWorld() const;
    int getActiveHitboxDamage() const;

    AABB getHurtboxWorld() const;

    bool isBlocking() const;

    void registerHitLanded();
    // Game calls this right after applying damage

public:
    // -----------------------------------------
    // Damage / Hit reaction
    // -----------------------------------------
    void applyHit(int damage, bool wasBlocked);

public:
    // -----------------------------------------
    // Getters
    // -----------------------------------------
    float getX() const;
    float getY() const;

    int getHealth() const;
    int getMaxHealth() const;

    bool isKO() const;
    bool getFacingRight() const;
};

#endif // CHARACTER_HPP