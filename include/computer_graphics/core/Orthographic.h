#ifndef CG_ORTHOGRAPHIC_H
#define CG_ORTHOGRAPHIC_H

#include "computer_graphics/core/Mat4.h"

namespace cg {

// ORTHOGRAPHIC projection: maps an axis-aligned box of world space onto
// Normalized Device Coordinates (-1..1 on every axis). Parallel lines stay
// parallel and there is NO perspective divide (w stays 1) — exactly what a
// 2D fighter needs.
//
// Drop-in replacement for the fixed-function call:
//   glOrtho(left, right, bottom, top, nearZ, farZ);
//
// Derivation, shown for X (Y and Z are symmetric):
//   world x in [left, right]  must map to  ndc x in [-1, +1]
//
//   ndc = (x - left) * 2 / (right - left) - 1
//       = x * 2/(right - left)  -  (right + left)/(right - left)
//         \___________/              \____________________/
//          scale (m[0])               offset (m[12])
//
// The Z row NEGATES (m[10] = -2/(far-near)): OpenGL looks down -Z, so the
// near plane ends up at NDC -1 after the negation. Geometry outside the box
// is clipped by OpenGL.
struct Orthographic
{
    float left;
    float right;
    float bottom;
    float top;
    float nearZ;
    float farZ;

    Orthographic(float left_, float right_, float bottom_, float top_,
                 float nearZ_ = -1.0f, float farZ_ = 1.0f)
        : left(left_), right(right_), bottom(bottom_), top(top_),
          nearZ(nearZ_), farZ(farZ_) {}

    Mat4 toMatrix() const
    {
        Mat4 result = Mat4::identity();

        // Scale each axis from the box extent to the 2-wide NDC range
        result.m[0]  =  2.0f / (right - left);
        result.m[5]  =  2.0f / (top - bottom);
        result.m[10] = -2.0f / (farZ - nearZ);

        // Then translate the box center onto the NDC origin
        result.m[12] = -((right + left) / (right - left));
        result.m[13] = -((top + bottom) / (top - bottom));
        result.m[14] = -((farZ + nearZ) / (farZ - nearZ));

        return result;
    }
};

} // namespace cg

#endif // CG_ORTHOGRAPHIC_H
