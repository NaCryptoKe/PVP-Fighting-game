#ifndef HITBOX_HPP
#define HITBOX_HPP

/*
 * instead of recomputing every frame twice (for two players)
 * just give it when starting and manipulate that one
*/
// Axis-aligned bounding box in world space (1920x1080 virtual coords).
struct AABB
{
    float left, right, bottom, top;
    // Instead of using right as left + width, just giving it the right hand side.
    // Same goes for top
};

struct HitBox
{
    float offsetX = 0.0f;       // Offset from feet centered for the player character
    float offsetY = 0.0f;
    float width   = 0.0f;
    float height  = 0.0f;

    AABB toWorld(float footX, float footY, bool facingRight) const;
    // Change from relative dimension to world coordinates
};

bool aabbOverlap(const AABB& a, const AABB& b);

#endif // HITBOX_HPP
