/// @file    Camera.hpp
/// @brief   Orbit camera using Qt Math with spherical coordinates.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include "Constants.hpp"
#include "Types.hpp"

namespace cad {

/// @brief Orbit camera with spherical coordinate control.
class Camera {
public:
    Camera();

    /// @brief Get view matrix.
    [[nodiscard]] Mat4 viewMatrix() const;

    /// @brief Get projection matrix.
    [[nodiscard]] Mat4 projectionMatrix() const;

    /// @brief Get combined view-projection matrix.
    [[nodiscard]] Mat4 viewProjectionMatrix() const;

    /// @brief Get camera position computed from orbit parameters.
    [[nodiscard]] Vec3 position() const;

    /// @brief Get camera target point.
    [[nodiscard]] Vec3 target() const { return m_target; }

    /// @brief Rotate camera around target.
    void orbit(float deltaYaw, float deltaPitch);

    /// @brief Pan camera perpendicular to view direction.
    void pan(float deltaX, float deltaY);

    /// @brief Zoom camera in/out.
    void zoom(float delta);

    /// @brief Reset camera to default view.
    void reset();

    /// @brief Frame bounding box in view.
    void frameBox(const AABB& bbox);

    /// @brief Set viewport dimensions.
    void setViewport(int width, int height);

    /// @brief Get viewport aspect ratio.
    [[nodiscard]] float aspectRatio() const;

    /// @brief Create ray from screen coordinates.
    [[nodiscard]] Ray screenToRay(int x, int y) const;

    [[nodiscard]] float distance() const { return m_distance; }
    [[nodiscard]] float yaw() const { return m_yaw; }
    [[nodiscard]] float pitch() const { return m_pitch; }
    [[nodiscard]] float fov() const { return m_fov; }

    void setTarget(const Vec3& target) { m_target = target; }
    void setDistance(float distance);
    void setFov(float fov) { m_fov = fov; }

private:
    void clampPitch();

    Vec3 m_target{0.0f, 0.0f, 0.0f};
    float m_distance{constants::DefaultCameraDistance};
    float m_yaw{45.0f};
    float m_pitch{30.0f};

    float m_fov{constants::DefaultFov};
    float m_nearPlane{constants::DefaultNearPlane};
    float m_farPlane{constants::DefaultFarPlane};

    int m_viewportWidth{1};
    int m_viewportHeight{1};
};

} // namespace cad
