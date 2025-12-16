/// @file    Camera.cpp
/// @brief   Implementation of orbit camera.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#include "Camera.hpp"
#include <algorithm>
#include <cmath>

namespace cad {

Camera::Camera() {
    reset();
}

Mat4 Camera::viewMatrix() const {
    Mat4 view;
    view.setToIdentity();
    view.lookAt(position(), m_target, Vec3(0.0f, 1.0f, 0.0f));
    return view;
}

Mat4 Camera::projectionMatrix() const {
    Mat4 proj;
    proj.setToIdentity();
    proj.perspective(m_fov, aspectRatio(), m_nearPlane, m_farPlane);
    return proj;
}

Mat4 Camera::viewProjectionMatrix() const {
    return projectionMatrix() * viewMatrix();
}

Vec3 Camera::position() const {
    const float yawRad = m_yaw * constants::DegToRad;
    const float pitchRad = m_pitch * constants::DegToRad;

    const float cosP = std::cos(pitchRad);
    const float sinP = std::sin(pitchRad);
    const float cosY = std::cos(yawRad);
    const float sinY = std::sin(yawRad);

    Vec3 offset(
        m_distance * cosP * sinY,
        m_distance * sinP,
        m_distance * cosP * cosY
    );

    return m_target + offset;
}

void Camera::orbit(float deltaYaw, float deltaPitch) {
    m_yaw += deltaYaw * constants::OrbitSensitivity;
    m_pitch += deltaPitch * constants::OrbitSensitivity;
    clampPitch();
}

void Camera::pan(float deltaX, float deltaY) {
    // Calculate camera right and up vectors
    Vec3 forward = (m_target - position()).normalized();
    Vec3 right = Vec3::crossProduct(forward, Vec3(0.0f, 1.0f, 0.0f)).normalized();
    Vec3 up = Vec3::crossProduct(right, forward).normalized();

    float panScale = m_distance * constants::PanSensitivity;
    m_target += right * (-deltaX * panScale);
    m_target += up * (deltaY * panScale);
}

void Camera::zoom(float delta) {
    m_distance -= delta * m_distance * constants::ZoomSensitivity;
    m_distance = std::clamp(m_distance, constants::MinCameraDistance, constants::MaxCameraDistance);
}

void Camera::reset() {
    m_target = Vec3(0.0f, 0.0f, 0.0f);
    m_distance = constants::DefaultCameraDistance;
    m_yaw = 45.0f;
    m_pitch = 30.0f;
    m_fov = constants::DefaultFov;
}

void Camera::frameBox(const AABB& bbox) {
    if (!bbox.isValid()) {
        return;
    }

    m_target = bbox.center();

    // Calculate distance to fit box in view
    Vec3 size = bbox.size();
    float maxDim = std::max({size.x(), size.y(), size.z()});
    float halfFovRad = (m_fov * 0.5f) * constants::DegToRad;
    m_distance = (maxDim * 0.5f) / std::tan(halfFovRad) * 1.5f;

    m_distance = std::clamp(m_distance, constants::MinCameraDistance, constants::MaxCameraDistance);
}

void Camera::setViewport(int width, int height) {
    m_viewportWidth = std::max(1, width);
    m_viewportHeight = std::max(1, height);
}

float Camera::aspectRatio() const {
    return static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight);
}

Ray Camera::screenToRay(int x, int y) const {
    float ndcX = (2.0f * static_cast<float>(x) / static_cast<float>(m_viewportWidth)) - 1.0f;
    float ndcY = 1.0f - (2.0f * static_cast<float>(y) / static_cast<float>(m_viewportHeight));

    Vec4 clipNear(ndcX, ndcY, -1.0f, 1.0f);
    Vec4 clipFar(ndcX, ndcY, 1.0f, 1.0f);

    Mat4 invVP = viewProjectionMatrix().inverted();
    Vec4 worldNear = invVP.map(clipNear);
    Vec4 worldFar = invVP.map(clipFar);

    Vec3 near3(worldNear.x() / worldNear.w(),
               worldNear.y() / worldNear.w(),
               worldNear.z() / worldNear.w());
    Vec3 far3(worldFar.x() / worldFar.w(),
              worldFar.y() / worldFar.w(),
              worldFar.z() / worldFar.w());

    return Ray{near3, (far3 - near3).normalized()};
}

void Camera::setDistance(float distance) {
    m_distance = std::clamp(distance, constants::MinCameraDistance, constants::MaxCameraDistance);
}

void Camera::clampPitch() {
    m_pitch = std::clamp(m_pitch, constants::MinPitch, constants::MaxPitch);
}

} // namespace cad
