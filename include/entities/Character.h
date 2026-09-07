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
    float frameAccumulator = 0;
    int frameCounter = 0;
    bool hasHit = false;

    float hitstunTimer = 0.0f;

    // Animation and sprite systems
    std::unordered_map<std::string, Animation> animations;
    Sprite sprite;

public:
    Character(float posX, float posY, float HP);

    // Load all stats and attacks from a CharacterData at once
    void applyData(const CharacterData& data);

    void update(float deltaTime);
    void render();
    void renderHitBox();

    void takeDamage(float damage);
    void onHit(float damage, float knockback, bool pushRight, float hitstunTime = 0.3f);
    void updateHitstun(float deltaTime);

    void moveFront();
    void moveBack();
    void stopX();
    void jump();
    void autoFace(float opponentX);
    void moveHitbox();

    void setBlocking(bool wantBlock);
    bool isBlocking() const;
    bool canAct() const;
    bool canMove() const;
    bool isDead() const;
    void setCrouching(bool wantCrouch);
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
        const std::string &name, float knockBackForce = 0.0f, bool blockable = true
    );

    void renderDamageBox(const std::string &name);
    void performAttack(const std::string &name);
    std::string getCurrentAttackName() const;
    void updateAttack(float deltaTime);
    // Advances the animation for the current state and feeds it to the sprite
    void updateAnimation(float deltaTime);
    bool isActiveAttack() const;
    bool getActiveAttackHitbox(AABB &outBox) const;
    float getCurrentAttackDamage() const;
    float getCurrentAttackKnockback() const;
    bool getHasHit() const;
    void markHit();

    void collision(float leftLimit, float rightLimit);
};

#endif // CHARACTER_H