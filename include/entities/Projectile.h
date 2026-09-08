#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "entities/Character.h"
#include "graphics/Animation.h"
#include "graphics/Sprite.h"

// A fireball: flies horizontally and hurts the first opponent it touches.
// Textures are loaded once into a template; every spawn is a lightweight copy.
class Projectile
{
public:
    // Loads the looping flight animation
    bool loadFromFiles(const char* folder, int frameCount, float duration, int startFrame = 1);

    // (Re)activates the projectile at a position, flying in a horizontal direction
    void spawn(float x, float y, float direction, int owner);

    // Moves the projectile; deactivates it on hit or when it leaves the stage
    void update(float deltaTime, Character& opponent);

    void render() const;
    bool isActive() const;
    int getOwner() const;

private:
    AABB getCollisionBox() const;

    Animation anim;
    float positionX = 0.0f;
    float positionY = 0.0f;
    float velocityX = 0.0f;
    float scale = 3.0f;
    float damage = 80.0f;
    float knockback = 300.0f;
    float hitstunTime = 0.35f;
    int owner = 0;
    bool active = false;
};

#endif // PROJECTILE_H
