#ifndef CHARACTER_H
#define CHARACTER_H

#include "entities/CharacterState.h"
#include "entities/CharacterRoster.h"
#include "utils/AABB.h"
#include "graphics/Animation.h"
#include "graphics/Sprite.h"

#include <unordered_map>
#include <string>

class Character
{
private:
    float positionX;
    float positionY;
    float velocityX;
    float velocityY;
    float jumpForce = 2571.43f;
    float walkSpeed = 450.0f;

    float maxHealth;
    float currentHealth;

    bool facingRight;
    CharacterState currentState;

    BOX hitbox;

    std::unordered_map<std::string, AttackData> attacks;
    std::string currentAttackName = "";
    std::string currentAttackAnimName;   // CROUCH_/JUMP_ variant actually playing
    float frameAccumulator = 0;
    int frameCounter = 0;
    bool hasHit = false;
    bool projectileSpawned = false;      // one fireball projectile per cast

    float hitstunTimer = 0.0f;
    std::string hitstunAnimOverride;     // e.g. "BACKWARD_THROW" after a throw
    float blockFlashTimer = 0.0f;        // short BLOCK_HIT flash while guarding
    float fireballCooldown = 0.0f;

    bool crouching = false;              // remembered for hurtbox height + reactions

    // Animation and sprite systems
    std::unordered_map<std::string, Animation> animations;
    std::string currentAnimName;         // animation currently playing (reset on switch)
    std::string victoryAnimKey = "VICTORY";
    Sprite sprite;

public:
    Character(float posX, float posY, float HP);

    // Load all stats and attacks from a CharacterData at once
    void applyData(const CharacterData& data);

    void update(float deltaTime);
    void render();
    void renderHitBox();

    void takeDamage(float damage);
    void onHit(const HitImpact& impact);
    void updateHitstun(float deltaTime);

    void moveFront();
    void moveBack();
    void stopX();
    void jump();
    void autoFace(float opponentX);
    void moveHitbox();

    // Derives BLOCK / CROUCH / CROUCH_BLOCK from the two held guard inputs
    void updateGuard(bool wantBlock, bool wantCrouch);
    bool isBlocking() const;
    bool canAct() const;
    bool canMove() const;
    bool isDead() const;
    void setVictory(bool matchWon);
    void resetForRound(float posX, float posY);

    void setPositionX(float posX);
    void setPositionY(float posY);
    void setVelocityX(float velX);
    void setVelocityY(float velY);
    void setFacingRight(bool facingRight);
    void setState(CharacterState state);
    void setHitBox(float offsetX, float offsetY, float width, float height);

    float getPositionX() const;
    float getPositionY() const;
    float getVelocityX() const;
    float getVelocityY() const;
    float getHealth() const;
    float getMaxHealth() const;
    bool isFacingRight() const;
    bool isGrounded() const;
    CharacterState getState() const;
    AABB getHitBox() const;

    bool loadAttack(
        AttackType type, int startupFrame, int activeFrame, int recoveryFrame,
        float damageAmount, float hboffsetX, float hboffsetY, float width, float height,
        const std::string &name, float knockBackForce = 0.0f, bool blockable = true,
        float hitstunTime = 0.3f
    );

    void renderDamageBox(const std::string &name);
    void performAttack(const std::string &name);
    // Fires once per fireball cast, when the startup frames finish
    bool consumeProjectileSpawn();
    std::string getCurrentAttackName() const;
    void updateAttack(float deltaTime);
    // Advances the animation for the current state and feeds it to the sprite
    void updateAnimation(float deltaTime);
    // Plays the named animation, restarting it whenever the animation changes
    void playAnimation(const std::string& name, float deltaTime);
    // True when the current velocity points the way the character faces
    bool isMovingForward() const;
    bool isActiveAttack() const;
    bool getActiveAttackHitbox(AABB &outBox) const;
    float getCurrentAttackDamage() const;
    float getCurrentAttackKnockback() const;
    float getCurrentAttackHitstun() const;
    bool isCurrentAttackBlockable() const;
    bool getHasHit() const;
    void markHit();

    void collision(float leftLimit, float rightLimit);
};

#endif // CHARACTER_H
