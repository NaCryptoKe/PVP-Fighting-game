#ifndef CAMERA_HPP
#define CAMERA_HPP

class Camera
{
private:
    float intensity;
    float duration;

    float offsetX;
    float offsetY;

public:
    Camera();

    void shake(float intensity, float duration);
    void update(float deltaTime);

    float getOffsetX() const;
    float getOffsetY() const;
};

#endif // CAMERA_HPP