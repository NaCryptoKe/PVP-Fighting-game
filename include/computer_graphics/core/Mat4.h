#ifndef CG_MAT4_H
#define CG_MAT4_H

#include "computer_graphics/core/Vec3.h"

namespace cg {

// 4x4 matrix stored COLUMN-MAJOR, exactly the layout OpenGL's matrix
// functions expect (glLoadMatrixf / glMultMatrixf read 16 floats column
// by column). The underlying array maps to the mathematical matrix like so:
//
//   | m[0]  m[4]  m[8]   m[12] |
//   | m[1]  m[5]  m[9]   m[13] |
//   | m[2]  m[6]  m[10]  m[14] |
//   | m[3]  m[7]  m[11]  m[15] |
//
// Vertices are COLUMN vectors and transform as  v' = M * v,  the same
// convention as the fixed-function pipeline. The translation therefore
// lives in m[12], m[13], m[14] (the last column).
struct Mat4
{
    float m[16];

    // Identity: leaves every vertex untouched.
    static Mat4 identity()
    {
        Mat4 result;
        for (int i = 0; i < 16; ++i) result.m[i] = 0.0f;
        result.m[0]  = 1.0f;   // scale X
        result.m[5]  = 1.0f;   // scale Y
        result.m[10] = 1.0f;   // scale Z
        result.m[15] = 1.0f;   // homogeneous w
        return result;
    }

    // Translation matrix: shifts every vertex by (t.x, t.y, t.z).
    static Mat4 translation(const Vec3& t)
    {
        Mat4 result = identity();
        result.m[12] = t.x;
        result.m[13] = t.y;
        result.m[14] = t.z;
        return result;
    }

    // Matrix product (this * rhs), written out component by component.
    // Column-major rule: result column j = this * (column j of rhs).
    // ORDER MATTERS: (A * B) applies B to the vertex first, then A —
    // this is what makes the TRS order in Transform meaningful.
    Mat4 operator*(const Mat4& rhs) const
    {
        Mat4 result;
        for (int column = 0; column < 4; ++column)
        {
            for (int row = 0; row < 4; ++row)
            {
                float sum = 0.0f;
                for (int k = 0; k < 4; ++k)
                    sum += m[k * 4 + row] * rhs.m[column * 4 + k];
                result.m[column * 4 + row] = sum;
            }
        }
        return result;
    }

    // Transforms a POINT (homogeneous w = 1): rotation, scale AND
    // translation all apply. The divide-by-w is skipped because every
    // matrix in this core is affine (and the orthographic projection
    // keeps w = 1 anyway).
    Vec3 transformPoint(const Vec3& point) const
    {
        return Vec3(
            m[0] * point.x + m[4] * point.y + m[8]  * point.z + m[12],
            m[1] * point.x + m[5] * point.y + m[9]  * point.z + m[13],
            m[2] * point.x + m[6] * point.y + m[10] * point.z + m[14]);
    }

    // Transforms a DIRECTION (homogeneous w = 0): rotation and scale only,
    // the translation column never contributes.
    Vec3 transformDirection(const Vec3& direction) const
    {
        return Vec3(
            m[0] * direction.x + m[4] * direction.y + m[8]  * direction.z,
            m[1] * direction.x + m[5] * direction.y + m[9]  * direction.z,
            m[2] * direction.x + m[6] * direction.y + m[10] * direction.z);
    }

    // Raw pointer for OpenGL: glLoadMatrixf / glMultMatrixf /
    // glUniformMatrix4fv all accept this layout directly.
    const float* data() const { return m; }
};

} // namespace cg

#endif // CG_MAT4_H
