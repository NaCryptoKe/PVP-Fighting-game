#ifndef HITBOX_HPP
#define HITBOX_HPP

// Axis-aligned bounding box in world space (1920x1080 virtual coords).
struct AABB
{
    float left, right, bottom, top;
};

struct HitBox
{
    float offsetX = 0.0f;
    float offsetY = 0.0f;
    float width   = 0.0f;
    float height  = 0.0f;

    AABB toWorld(float footX, float footY, bool facingRight) const;
};

bool aabbOverlap(const AABB& a, const AABB& b);

#endif // HITBOX_HPP