#ifndef CG_VEC2_H
#define CG_VEC2_H

#include <cmath>

namespace cg {

// 2-component vector for 2D positions, sizes and directions.
// Part of the hand-rolled math core for the computer-graphics assignment:
// every operation here is written out explicitly instead of using a library.
struct Vec2
{
    float x = 0.0f;
    float y = 0.0f;

    Vec2() = default;
    Vec2(float x_, float y_) : x(x_), y(y_) {}

    Vec2 operator+(const Vec2& rhs) const { return Vec2(x + rhs.x, y + rhs.y); }
    Vec2 operator-(const Vec2& rhs) const { return Vec2(x - rhs.x, y - rhs.y); }
    Vec2 operator*(float scalar)  const { return Vec2(x * scalar, y * scalar); }

    float length() const { return std::sqrt(x * x + y * y); }

    Vec2 normalized() const
    {
        const float len = length();
        return (len > 0.0f) ? Vec2(x / len, y / len) : Vec2();
    }
};

} // namespace cg

#endif // CG_VEC2_H
