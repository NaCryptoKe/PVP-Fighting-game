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

    AABB box;

    // Static function to test if two AABBs intersect
    static bool intersects(const AABB& a, const AABB& b) 
    {
        return (
                a.left < b.right &&
                a.right > b.left &&
                a.bottom < b.top &&
                a.top > b.bottom
            );
    }

    AABB toWorld(float footX, float footY, bool facingRight) const
    {
        float worldOffsetX = facingRight ? offsetX : -offsetX - width;

        AABB result;
        result.left    = footX + worldOffsetX;
        result.right   = result.left + width;
        result.bottom  = footY + offsetY;
        result.top     = result.bottom + height;
        return result;
    }
};

#endif // AABB_H