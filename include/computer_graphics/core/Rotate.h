#ifndef CG_ROTATE_H
#define CG_ROTATE_H

#include <cmath>

#include "computer_graphics/core/Mat4.h"

namespace cg {

// All public APIs take DEGREES (gameplay code thinks in degrees); the
// trigonometry below converts to radians exactly once.
inline float toRadians(float degrees)
{
    return degrees * 3.14159265358979323846f / 180.0f;
}

// Full 3D rotations, right-handed: counter-clockwise when looking from the
// positive axis back towards the origin.

// Rotation about X: tilts +Y towards +Z.
inline Mat4 rotateAboutX(float degrees)
{
    const float c = std::cos(toRadians(degrees));
    const float s = std::sin(toRadians(degrees));

    Mat4 result = Mat4::identity();
    result.m[5]  =  c;  result.m[6]  =  s;
    result.m[9]  = -s;  result.m[10] =  c;
    return result;
}

// Rotation about Y: tilts +Z towards +X.
inline Mat4 rotateAboutY(float degrees)
{
    const float c = std::cos(toRadians(degrees));
    const float s = std::sin(toRadians(degrees));

    Mat4 result = Mat4::identity();
    result.m[0]  =  c;  result.m[2]  = -s;
    result.m[8]  =  s;  result.m[10] =  c;
    return result;
}

// Rotation about Z: the 2D rotation this game actually draws with — spins
// the XY plane counter-clockwise (screen space has +Y pointing up).
//
//   |  c s 0 0 |
//   | -s c 0 0 |
//   |  0 0 1 0 |
//   |  0 0 0 1 |
//
// (Top-left 2x2 block is the classic 2D rotation matrix.)
inline Mat4 rotateAboutZ(float degrees)
{
    const float c = std::cos(toRadians(degrees));
    const float s = std::sin(toRadians(degrees));

    Mat4 result = Mat4::identity();
    result.m[0]  =  c;  result.m[1]  =  s;
    result.m[4]  = -s;  result.m[5]  =  c;
    return result;
}

// Thin wrapper so a plain "rotate this many degrees" reads naturally inside
// cg::Transform (which rotates about Z, i.e. within the 2D screen plane).
struct Rotate
{
    float degrees = 0.0f;

    Rotate() = default;
    explicit Rotate(float degreesAboutZ) : degrees(degreesAboutZ) {}

    Mat4 toMatrix() const { return rotateAboutZ(degrees); }
};

} // namespace cg

#endif // CG_ROTATE_H
