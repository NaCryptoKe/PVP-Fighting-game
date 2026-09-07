#ifndef AABB_H
#define AABB_H

struct AABB
{
    float left;
    float right;
    float top;
    float bottom;
};

struct BOX 
{
    float offsetX;
    float offsetY;
    float width;
    float height;

    AABB playerBox;

    // Static function to test if two AABBs intersect
    static bool intersects(const AABB& player1, const AABB& player2) 
    {
        return (
                player1.left < player2.right &&
                player1.right > player2.left &&
                player1.bottom < player2.top &&
                player1.top > player2.bottom
            );
    }

    AABB toWorld(float footX, float footY, bool facingRight) const
    {
        float worldOffsetX = facingRight ? offsetX : -offsetX - width;

        AABB box;
        box.left    = footX + worldOffsetX;
        box.right   = box.left + width;
        box.bottom  = footY + offsetY;
        box.top     = box.bottom + height;
        return box;
    }
};

#endif // AABB_H