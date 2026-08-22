#include "src/HitBox.hpp"

AABB HitBox::toWorld(float footX, float footY, bool facingRight) const {
    /*
    * What the arguments are:
    * footX the character's feet X coordinates
    * footY the character's feet Y coordinates
    * facingRight: whether it is facing right or left
    */
    // Facing left mirrors the box around the foot anchor.
    float worldOffsetX = facingRight ? offsetX : -offsetX - width;

    AABB box;
    box.left    = footX + worldOffsetX;
    box.right   = box.left + width;
    box.bottom  = footY + offsetY;
    box.top     = box.bottom + height;
    return box;
}

bool aabbOverlap(const AABB& a, const AABB& b)
{
    return  a.left < b.right && 
            a.right > b.left &&
            a.bottom < b.top && 
            a.top > b.bottom;
}