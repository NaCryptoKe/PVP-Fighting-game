#ifndef CG_TRANSFORM_H
#define CG_TRANSFORM_H

#include "computer_graphics/core/Mat4.h"
#include "computer_graphics/core/Scale.h"
#include "computer_graphics/core/Rotate.h"
#include "computer_graphics/core/Vec2.h"

namespace cg {

// TRANSFORM: the classic TRS model matrix — Position, Rotation, Scale.
//
// toMatrix() composes  T * R * S  (translate, rotate, scale), mirroring the
// fixed-function sequence glTranslatef -> glRotatef -> glScalef. With column
// vectors the RIGHT-most matrix touches the vertex first, so the order means:
//
//   1. S scales the raw local geometry around the pivot (the origin),
//   2. R rotates that scaled result around the pivot,
//   3. T finally places the object into the world.
//
// Any other order breaks this: scaling AFTER rotating with a non-uniform
// scale shears the object, and translating before scaling would make the
// position itself scale.
struct Transform
{
    Vec3 position{ 0.0f, 0.0f, 0.0f };   // world-space location of the pivot
    float rotationDegrees = 0.0f;        // rotation about Z (2D screen plane)
    Vec3 scale{ 1.0f, 1.0f, 1.0f };      // 1 = original size, negative mirrors

    Transform() = default;
    Transform(const Vec3& position_, float rotationDegrees_, const Vec3& scale_)
        : position(position_), rotationDegrees(rotationDegrees_), scale(scale_) {}

    // The composed model matrix:  T * R * S
    Mat4 toMatrix() const
    {
        return Mat4::translation(position) *
               Rotate(rotationDegrees).toMatrix() *
               Scale(scale).toMatrix();
    }

    // Moves the pivot by a world-space delta
    void translate(const Vec3& delta) { position = position + delta; }
    void translate(const Vec2& delta) { position.x += delta.x; position.y += delta.y; }
};

} // namespace cg

#endif // CG_TRANSFORM_H
