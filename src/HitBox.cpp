#include "src/HitBox.hpp"

AABB HitBox::toWorld(float footX, float footY, bool facingRight) const
{
    // Facing left mirrors the box around the foot anchor.
    float ox = facingRight ? offsetX : -offsetX - width;

    AABB box;
    box.left   = footX + ox;
    box.right  = box.left + width;
    box.bottom = footY + offsetY;
    box.top    = box.bottom + height;
    return box;
}

bool aabbOverlap(const AABB& a, const AABB& b)
{
    return a.left < b.right && a.right > b.left &&
           a.bottom < b.top && a.top > b.bottom;
}