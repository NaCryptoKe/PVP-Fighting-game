#include "entities/Projectile.h"

#include "entities/Camera.h"
#include "render/Renderer.h"

bool Projectile::loadFromFiles(const char* folder, int frameCount, float duration, int startFrame)
{
    return anim.loadFromFiles(folder, frameCount, duration, true, startFrame);
}

void Projectile::spawn(float x, float y, float direction, int ownerCharacter)
{
    positionX = x;
    positionY = y;
    velocityX = direction * 850.0f;
    owner = ownerCharacter;
    active = true;
    anim.reset();
}

// Fireball collision box: a generous box around the sprite visual
AABB Projectile::getCollisionBox() const
{
    AABB box;
    box.left   = positionX - 70.0f;
    box.right  = positionX + 70.0f;
    box.bottom = positionY + 10.0f;
    box.top    = positionY + 90.0f;
    return box;
}

void Projectile::update(float deltaTime, Character& opponent)
{
    if (!active) return;

    anim.update(deltaTime);
    positionX += velocityX * deltaTime;

    // Cull once it leaves the stage
    if (positionX < Camera::STAGE_LEFT - 200.0f || positionX > Camera::STAGE_RIGHT + 200.0f)
    {
        active = false;
        return;
    }

    if (BOX::intersects(getCollisionBox(), opponent.getHitBox()))
    {
        HitImpact impact;
        impact.damage      = damage;
        impact.knockback   = knockback;
        impact.pushRight   = velocityX > 0.0f;
        impact.hitstunTime = hitstunTime;
        impact.blockable   = true;   // fireballs can be blocked for chip damage

        opponent.onHit(impact);
        active = false;
    }
}

void Projectile::render() const
{
    if (!active) return;

    TextureData tex = anim.getCurrentTexture();
    if (tex.id == 0) return;

    // Flip the sprite to match the flight direction
    Renderer::drawFighterSprite(tex, positionX, positionY, scale, velocityX < 0.0f);
}

bool Projectile::isActive() const { return active; }
int Projectile::getOwner() const { return owner; }
