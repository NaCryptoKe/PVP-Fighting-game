#ifndef PLAYER_H
#define PLAYER_H

#include "entities/PlayerState.h"
#include "utils/AABB.h"

#include <unordered_map>
#include <string>

class Player
{
private:
    float positionX;
    float positionY;
    float velocityX;
    float velocityY;
    const float jumpForce = 2571.43f;
    const float walk = 450.0f;

    float maxHealth;
    float currentHealth;

    bool facingRight;
    PlayerState currentState;

    BOX hitbox;

    std::unordered_map<std::string, AttackData> attacks;
    std::string currentAttackName = "";
    float frameAccumulator = 0;
    int frameCounter = 0;
    bool hasHit = false;

    float hitstunTimer = 0.0f;

public:
    Player(float posX, float posY, float HP);

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
    void setState(PlayerState state);
    void setHitBox(float offsetX, float offsetY, float width, float height);

    float getPositionX() const;
    float getPositionY() const;
    float getVelocityX() const;
    float getVelocityY() const;
    float getHealth() const;
    float getMaxHealth() const;
    bool isFacingRight() const;
    bool isGrounded() const;
    PlayerState getState() const;
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
    bool isActiveAttack() const;
    bool getActiveAttackHitbox(AABB &outBox) const;
    float getCurrentAttackDamage() const;
    float getCurrentAttackKnockback() const;
    bool getHasHit() const;
    void markHit();

    void collision(float leftLimit, float rightLimit);
};

#endif // PLAYER_H