#ifndef CG_VEC3_H
#define CG_VEC3_H

#include <cmath>

namespace cg {

// 3-component vector used for positions, scales and directions inside the
// math core. Written from scratch for the computer-graphics assignment.
struct Vec3
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Vec3() = default;
    Vec3(float x_, float y_, float z_ = 0.0f) : x(x_), y(y_), z(z_) {}

    Vec3 operator+(const Vec3& rhs) const { return Vec3(x + rhs.x, y + rhs.y, z + rhs.z); }
    Vec3 operator-(const Vec3& rhs) const { return Vec3(x - rhs.x, y - rhs.y, z - rhs.z); }
    Vec3 operator*(float scalar)  const { return Vec3(x * scalar, y * scalar, z * scalar); }

    float length() const { return std::sqrt(x * x + y * y + z * z); }

    Vec3 normalized() const
    {
        const float len = length();
        return (len > 0.0f) ? Vec3(x / len, y / len, z / len) : Vec3();
    }
};

} // namespace cg

#endif // CG_VEC3_H
