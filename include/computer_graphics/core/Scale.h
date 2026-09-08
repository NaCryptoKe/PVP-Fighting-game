#ifndef CG_SCALE_H
#define CG_SCALE_H

#include "computer_graphics/core/Mat4.h"
#include "computer_graphics/core/Vec3.h"

namespace cg {

// SCALE: multiplies vertex coordinates by a factor per axis, around the
// ORIGIN of the local space. That is exactly why a model transform always
// applies scale FIRST (order T * R * S): the object scales around its own
// pivot instead of the world origin.
//
// A NEGATIVE factor mirrors the geometry on that axis. The sprite flip in
// this game is precisely that: scale.x = -width.
struct Scale
{
    float x = 1.0f;
    float y = 1.0f;
    float z = 1.0f;

    Scale() = default;
    explicit Scale(float uniform) : x(uniform), y(uniform), z(uniform) {}
    Scale(float x_, float y_, float z_ = 1.0f) : x(x_), y(y_), z(z_) {}
    explicit Scale(const Vec3& v) : x(v.x), y(v.y), z(v.z) {}

    // Diagonal matrix:
    //   | x 0 0 0 |
    //   | 0 y 0 0 |
    //   | 0 0 z 0 |
    //   | 0 0 0 1 |
    Mat4 toMatrix() const
    {
        Mat4 result = Mat4::identity();
        result.m[0]  = x;
        result.m[5]  = y;
        result.m[10] = z;
        return result;
    }
};

} // namespace cg

#endif // CG_SCALE_H
