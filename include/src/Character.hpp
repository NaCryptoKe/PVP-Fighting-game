#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "src/Renderer.hpp"
#include "src/Sprite.hpp"
#include "src/Animation.hpp"
#include "src/Input.hpp"
#include "src/HitBox.hpp"

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
    // Character position and info
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
    // Character movement
    // -----------------------------------------
    HitBox standingHurtbox;
    HitBox crouchingHurtbox;

private:
    // The image of the character 
    Sprite sprite;

    // -----------------------------------------
    // Animations
    // -----------------------------------------
    // All animation of the character
    Animation idleAnim;
    Animation jumpAnim;
    Animation walkAnim;
    Animation crouchAnim;
    Animation blockAnim;
    Animation hitAnim;

    AttackData attacks[4]; // indexed by (int)AttackType

    Animation* currentAnim;

    AttackType currentAttack;
    bool hasHitThisAttack;

    
    float hitStunTimer;
    static constexpr float HIT_STUN_DURATION = 0.35f;

public:
    Character();

    bool init(
        const char* idleFolder, int idleFrames,
        const char* jumpFolder, int jumpFrames
    );

    // All optional - if never loaded, that state/attack falls back to
    // idle (walk/crouch/block) or simply does nothing when triggered
    // (attacks), same graceful-degradation pattern used everywhere.
    bool loadWalkAnimation(const char* folder, int frames);
    bool loadCrouchAnimation(const char* folder, int frames);
    bool loadBlockAnimation(const char* folder, int frames);
    bool loadHitAnimation(const char* folder, int frames);
    bool loadJumpAnimation(const char* folder, int frames);

    bool loadAttack(
        AttackType type,
        const char* folder, int frameCount, float frameDuration,
        int activeStartFrame, int activeEndFrame,
        int damage,
        float hbOffsetX, float hbOffsetY, float hbWidth, float hbHeight
    );

    void setGroundY(float ground);
    void setHurtboxes(float standW, float standH, float crouchW, float crouchH);

    void handleInput(const InputManager& input);
    void faceToward(float opponentX); // auto-face; skipped mid-attack/hitstun/KO

    void update(float deltaTime);
    void render();

    void setState(CharacterState newState);
    void setPosition(float px, float py);
    void setFacing(bool right);
    void setScale(int scale);

    // --- Combat queries, used by Game for hit detection ---
    bool hasActiveHitbox() const;
    AABB getActiveHitboxWorld() const;
    int getActiveHitboxDamage() const;
    AABB getHurtboxWorld() const;
    bool isBlocking() const;
    void registerHitLanded(); // Game calls this right after applying damage

    void applyHit(int damage, bool wasBlocked);

    float getX() const;
    float getY() const;
    int getHealth() const;
    int getMaxHealth() const;
    bool isKO() const;
    bool getFacingRight() const;
};

#endif // CHARACTER_HPP