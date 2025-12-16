/// @file    Constants.hpp
/// @brief   Named constexpr constants for the application.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include <cstddef>
#include <numbers>

namespace cad::constants {

inline constexpr float Pi = std::numbers::pi_v<float>;
inline constexpr float TwoPi = 2.0f * Pi;
inline constexpr float HalfPi = Pi / 2.0f;
inline constexpr float QuarterPi = Pi / 4.0f;
inline constexpr float DegToRad = Pi / 180.0f;
inline constexpr float RadToDeg = 180.0f / Pi;

inline constexpr float DefaultFov = 45.0f;
inline constexpr float DefaultNearPlane = 0.1f;
inline constexpr float DefaultFarPlane = 1000.0f;
inline constexpr float DefaultCameraDistance = 10.0f;

inline constexpr float MinCameraDistance = 1.0f;
inline constexpr float MaxCameraDistance = 500.0f;

inline constexpr float OrbitSensitivity = 0.3f;
inline constexpr float PanSensitivity = 0.01f;
inline constexpr float ZoomSensitivity = 0.1f;
inline constexpr float WheelDeltaFactor = 120.0f;

inline constexpr float MinPitch = -89.0f;
inline constexpr float MaxPitch = 89.0f;

inline constexpr int DefaultMsaaSamples = 4;
inline constexpr float DefaultLineWidth = 1.5f;
inline constexpr float SelectionHighlightIntensity = 1.2f;

inline constexpr int DefaultSphereStacks = 32;
inline constexpr int DefaultSphereSectors = 32;
inline constexpr int DefaultTorusMajorSegments = 48;
inline constexpr int DefaultTorusMinorSegments = 24;

inline constexpr int SidebarMinWidth = 250;
inline constexpr int SidebarMaxWidth = 400;

inline constexpr std::size_t StlHeaderSize = 80;
inline constexpr std::size_t StlTriangleSize = 50;

} // namespace cad::constants
