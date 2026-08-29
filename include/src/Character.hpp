#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "src/Sprite.hpp"
#include "src/Animation.hpp"
#include "src/Input.hpp"
#include "src/HitBox.hpp"
#include "src/AudioManager.hpp"

#include <unordered_map>
#include <string>
#include <memory>

// ============================================================
// Audio States
// ============================================================

enum class AudioState
{
    KO,
    BLOCK,
    JUMP,
    ATTACK,
    IDLE
};

// ============================================================
// Character States
// ============================================================

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


// ============================================================
// Attack Types
// ============================================================

enum class AttackType
{
    LIGHT_PUNCH,  // CROSS
    HEAVY_PUNCH,  // CIRCLE
    LIGHT_KICK,   // SQUARE
    HEAVY_KICK    // TRIANGLE
};


// ============================================================
// Attack Data
// ============================================================

struct AttackData
{
    Animation anim;
    bool loaded = false;

    // Frames during which the attack can hit.
    int activeStartFrame = 0;
    int activeEndFrame   = 0;

    int damage = 0;

    // Authored in facing-right local space.
    HitBox hitbox;
};


// ============================================================
// Character
// ============================================================

class Character
{
private:

    // ========================================================
    // 1. Character State / Position
    // ========================================================

    float x;
    float y;

    float velocityX;
    float velocityY;

    bool facingRight;
    CharacterState currentState;
    AudioState audioState;


    // ========================================================
    // 2. Health
    // ========================================================

    int health;
    int maxHealth;


    // ========================================================
    // 3. Movement / Physics
    // ========================================================

    float groundY;
    float gravity;
    float jumpSpeed;
    float walkSpeed;


    // ========================================================
    // 4. Collision / Hurtboxes
    // ========================================================

    HitBox standingHurtbox;
    HitBox crouchingHurtbox;

    HitBox hitBox;
    HitBox blockBox;


    // ========================================================
    // 5. Rendering
    // ========================================================

    Sprite sprite;


    // ========================================================
    // 6. Animations
    // ========================================================

    std::unordered_map<CharacterState, Animation> animations;


    // ========================================================
    // 7. Attacks
    // ========================================================

    std::unordered_map<AttackType, AttackData> attacks;

    AttackType currentAttack;
    bool hasHitThisAttack;

    void startAttack(AttackType type);

    // ========================================================
    // 8. Hit Stun
    // ========================================================

    float hitStunTimer;

    static constexpr float HIT_STUN_DURATION = 0.35f;

    // ========================================================
    // 9. Audio
    // ========================================================
    // Held via unique_ptr: AudioManager owns a live audio-engine
    // handle (non-copyable), and has no move constructor either
    // (a user-declared destructor suppresses the implicit one).
    // unique_ptr gives Character a move ctor/assignment "for free"
    // without needing to touch AudioManager's internals - required
    // for CharacterRoster::createChunLi() to return by value.
    std::unique_ptr<AudioManager> audio;

    std::unordered_map<AudioState, std::string> sfxs;

public:

    // ========================================================
    // Constructor
    // ========================================================

    Character();


public:

    // ========================================================
    // Initialization
    // ========================================================

    bool init();

public:

    // ========================================================
    // Audio
    // ========================================================
    bool addSfx(AudioState state, const char* filepath);
    bool setVolume(float volume);
    bool playSfx(AudioState state);

public:

    // ========================================================
    // Animation Loading
    // ========================================================

    bool loadIdleAnimation(
        const char* folder,
        int frames,
        float duration
    );

    bool loadWalkAnimation(
        const char* folder,
        int frames,
        float duration
    );

    bool loadJumpAnimation(
        const char* folder,
        int frames,
        float duration
    );

    bool loadCrouchAnimation(
        const char* folder,
        int frames,
        float duration
    );

    bool loadBlockAnimation(
        const char* folder,
        int frames,
        float duration
    );

    bool loadHitStunAnimation(
        const char* folder,
        int frames,
        float duration
    );

    bool loadKOAnimation(
        const char* folder,
        int frames,
        float duration
    );

    bool loadAttack(
        AttackType type,
        const char* folder,
        int frames,
        float duration,
        int activeStartFrame,
        int activeEndFrame,
        int damage,
        float hbOffsetX,
        float hbOffsetY,
        float hbWidth,
        float hbHeight
    );


public:

    // ========================================================
    // Movement / Collision Setup
    // ========================================================

    void setGroundY(float ground);

    void setHurtboxes(
        float standW,
        float standH,
        float crouchW,
        float crouchH
    );


public:

    // ========================================================
    // Input / Facing
    // ========================================================

    void handleInput(const InputManager& input);

    void faceToward(float opponentX);
    // Automatically faces the opponent.
    // Does not change facing during attack, hit stun, or KO.


public:

    // ========================================================
    // Update / Rendering
    // ========================================================

    void update(float deltaTime);

    void render();


public:

    // ========================================================
    // State / Position Control
    // ========================================================

    void setState(CharacterState newState);

    void setPosition(float px, float py);

    void setFacing(bool right);

    void setScale(float scale);


public:

    // ========================================================
    // Combat Queries
    // Used by Game for hit detection.
    // ========================================================

    bool hasActiveHitbox() const;

    AABB getActiveHitboxWorld() const;

    int getActiveHitboxDamage() const;

    AABB getHurtboxWorld() const;

    bool isBlocking() const;


public:

    // ========================================================
    // Combat Events
    // ========================================================

    void registerHitLanded();
    // Game calls this after the attack successfully hits.


public:

    // ========================================================
    // Damage / Hit Reaction
    // ========================================================

    void applyHit(int damage, bool wasBlocked);


public:

    // ========================================================
    // Getters
    // ========================================================

    float getX() const;
    float getY() const;

    int getHealth() const;
    int getMaxHealth() const;

    bool isKO() const;
    bool getFacingRight() const;
};


#endif // CHARACTER_HPP
