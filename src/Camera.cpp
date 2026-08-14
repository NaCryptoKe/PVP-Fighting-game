#include "src/Camera.h"

Camera::Camera()
    : m_position(glm::vec3(0.0f, 2.0f, 10.0f)),
      m_target(glm::vec3(0.0f, 2.0f, 0.0f)),
      m_up(glm::vec3(0.0f, 1.0f, 0.0f)),
      m_mode(ProjectionMode::PERSPECTIVE),
      m_fov(45.0f),
      m_aspectRatio(16.0f / 9.0f),
      m_nearPlane(0.1f),
      m_farPlane(100.0f),
      m_orthoWidth(16.0f),
      m_orthoHeight(9.0f) {}

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up)
    : m_position(position),
      m_target(target),
      m_up(up),
      m_mode(ProjectionMode::PERSPECTIVE),
      m_fov(45.0f),
      m_aspectRatio(16.0f / 9.0f),
      m_nearPlane(0.1f),
      m_farPlane(100.0f),
      m_orthoWidth(16.0f),
      m_orthoHeight(9.0f) {}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(m_position, m_target, m_up);
}

glm::mat4 Camera::GetProjectionMatrix() const {
    if (m_mode == ProjectionMode::PERSPECTIVE) {
        return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
    } else {
        float halfW = m_orthoWidth * 0.5f;
        float halfH = m_orthoHeight * 0.5f;
        return glm::ortho(-halfW, halfW, -halfH, halfH, m_nearPlane, m_farPlane);
    }
}

void Camera::SetPosition(const glm::vec3& position) {
    m_position = position;
}

void Camera::SetTarget(const glm::vec3& target) {
    m_target = target;
}

void Camera::SetUpVector(const glm::vec3& up) {
    m_up = up;
}

void Camera::SetPerspective(float fovDegrees, float aspectRatio, float nearPlane, float farPlane) {
    m_mode = ProjectionMode::PERSPECTIVE;
    m_fov = fovDegrees;
    m_aspectRatio = aspectRatio;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
}

void Camera::SetOrthographic(float width, float height, float nearPlane, float farPlane) {
    m_mode = ProjectionMode::ORTHOGRAPHIC;
    m_orthoWidth = width;
    m_orthoHeight = height;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
}

void Camera::UpdateAspectRatio(float width, float height) {
    if (height == 0.0f) height = 1.0f; // Prevent divide-by-zero
    m_aspectRatio = width / height;

    // Scale orthographic width to preserve aspect ratio without stretching
    if (m_mode == ProjectionMode::ORTHOGRAPHIC) {
        m_orthoWidth = m_orthoHeight * m_aspectRatio;
    }
}

void Camera::TrackCharacter2D(const glm::vec2& characterPosXY, float cameraDistanceZ) {
    // Keep camera perpendicular to character position on Z = 0 plane
    m_target = glm::vec3(characterPosXY.x, characterPosXY.y, 0.0f);
    m_position = glm::vec3(characterPosXY.x, characterPosXY.y, cameraDistanceZ);
}

// Camera.cpp
void Camera::TrackTwoCharacters2D(const glm::vec2& char1PosXY, const glm::vec2& char2PosXY,
                                   float baseDistanceZ, float minDistanceZ, float maxDistanceZ,
                                   float paddingFactor) {
    // 1. Target = midpoint between the two fighters
    glm::vec2 midpoint = (char1PosXY + char2PosXY) * 0.5f;
    m_target = glm::vec3(midpoint.x, midpoint.y, 0.0f);

    // 2. Horizontal separation drives zoom — this is the width the camera must fit
    float separation = glm::abs(char1PosXY.x - char2PosXY.x);

    // 3. Convert desired on-screen width into a required camera distance.
    //    Using the perspective FOV: width_visible_at_Z = 2 * Z * tan(fov/2)
    //    So solving for Z:          Z = width_visible / (2 * tan(fov/2))
    float halfFovRad = glm::radians(m_fov) * 0.5f;
    float requiredDistance = (separation * paddingFactor) / (2.0f * glm::tan(halfFovRad));

    // 4. Clamp so we never get uncomfortably close or absurdly far
    float distanceZ = glm::clamp(glm::max(requiredDistance, baseDistanceZ),
                                  minDistanceZ, maxDistanceZ);

    // 5. Position camera at that distance, still centered on the midpoint
    m_position = glm::vec3(midpoint.x, midpoint.y, distanceZ);
}