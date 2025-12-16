/// @file    TorusGeometry.hpp
/// @brief   Torus primitive generator.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include "Constants.hpp"
#include "MeshGeometry.hpp"
#include <cmath>

namespace cad {

/// @brief Generates torus geometry with UV mapping.
class TorusGeometry {
public:
    [[nodiscard]] static GeometryData generate(
        float majorRadius = 1.0f,
        float minorRadius = 0.3f,
        int majorSegments = constants::DefaultTorusMajorSegments,
        int minorSegments = constants::DefaultTorusMinorSegments
    ) {
        GeometryData data;
        const std::size_t vertexCount = static_cast<std::size_t>((majorSegments + 1) * (minorSegments + 1));
        data.vertices.reserve(vertexCount);
        data.indices.reserve(static_cast<std::size_t>(majorSegments * minorSegments * 6));

        for (int i = 0; i <= majorSegments; ++i) {
            const float u = static_cast<float>(i) / static_cast<float>(majorSegments);
            const float theta = u * constants::TwoPi;
            const float cosTheta = std::cos(theta);
            const float sinTheta = std::sin(theta);

            for (int j = 0; j <= minorSegments; ++j) {
                const float v = static_cast<float>(j) / static_cast<float>(minorSegments);
                const float phi = v * constants::TwoPi;
                const float cosPhi = std::cos(phi);
                const float sinPhi = std::sin(phi);

                const float x = (majorRadius + minorRadius * cosPhi) * cosTheta;
                const float y = minorRadius * sinPhi;
                const float z = (majorRadius + minorRadius * cosPhi) * sinTheta;

                const Vec3 pos{x, y, z};

                const float nx = cosPhi * cosTheta;
                const float ny = sinPhi;
                const float nz = cosPhi * sinTheta;
                const Vec3 normal{nx, ny, nz};

                const Vec2 texCoord{u, v};

                data.vertices.push_back({pos, normal.normalized(), texCoord});
            }
        }

        for (int i = 0; i < majorSegments; ++i) {
            for (int j = 0; j < minorSegments; ++j) {
                const std::uint32_t current = static_cast<std::uint32_t>(i * (minorSegments + 1) + j);
                const std::uint32_t next = current + static_cast<std::uint32_t>(minorSegments + 1);

                data.indices.push_back(current);
                data.indices.push_back(next);
                data.indices.push_back(current + 1);

                data.indices.push_back(current + 1);
                data.indices.push_back(next);
                data.indices.push_back(next + 1);
            }
        }

        return data;
    }
};

} // namespace cad
