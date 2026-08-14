/*
 * The camera is defined by two independent sets of parameters
 * 1. View Parameters: where the camera is, and how is it oriented
 * (m_position, m_target, m_up)
 * 2. Projection parameters: how 3D space is flattened onto the screen
 * (perspective vs. orthographic, FOV, aspect ration, near/far planes)
 * 
 * further information can be found on this book
 * https://gabrielgambetta.com/computer-graphics-from-scratch/09-perspective-projection.html
*/

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// ============================
// Projection parameter
// ============================
enum class ProjectionMode {
    PERSPECTIVE,
    ORTHOGRAPHIC
};

class Camera {
public:
    // Constructors
    Camera();
    Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

    // Matrix Generators
    glm::mat4 GetViewMatrix() const; // world into camera-relative coordinates
    glm::mat4 GetProjectionMatrix() const; // Camera space to NDC

    // Position & Orientation Setters
    void SetPosition(const glm::vec3& position);
    void SetTarget(const glm::vec3& target);
    void SetUpVector(const glm::vec3& up);

    // Projection Mode Setup
    void SetPerspective(float fovDegrees, float aspectRatio, float nearPlane, float farPlane);
    void SetOrthographic(float width, float height, float nearPlane, float farPlane);
    void UpdateAspectRatio(float width, float height);

    // 2.5D Side-Scroller Helper
    // Locks camera to follow character on XY plane while maintaining distance on Z
    void TrackCharacter2D(const glm::vec2& characterPosXY, float cameraDistanceZ);

    // Getters
    glm::vec3 GetPosition() const { return m_position; }
    glm::vec3 GetTarget() const { return m_target; }
    ProjectionMode GetProjectionMode() const { return m_mode; }

    // Tracker for the two players
    void TrackTwoCharacters2D(
        const glm::vec2& char1PosXY, const glm::vec2& char2PosXY,
        float baseDistanceZ, float minDistanceZ, float maxDistanceZ,
        float paddingFactor = 1.3f
    );

private:
    // View Parameters
    glm::vec3 m_position;
    glm::vec3 m_target;
    glm::vec3 m_up;

    // Projection Parameters
    ProjectionMode m_mode;
    float m_fov;
    float m_aspectRatio;
    float m_nearPlane;
    float m_farPlane;

    // Orthographic Dimensions
    float m_orthoWidth;
    float m_orthoHeight;
};

#endif // CAMERA_H