/// @file    SphereGeometry.hpp
/// @brief   Sphere primitive generator.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include "Constants.hpp"
#include "MeshGeometry.hpp"
#include <cmath>

namespace cad {

/// @brief Generates sphere geometry with UV mapping.
class SphereGeometry {
public:
    [[nodiscard]] static GeometryData generate(
        float radius = 1.0f,
        int stacks = constants::DefaultSphereStacks,
        int sectors = constants::DefaultSphereSectors
    ) {
        GeometryData data;
        data.vertices.reserve(static_cast<std::size_t>((stacks + 1) * (sectors + 1)));
        data.indices.reserve(static_cast<std::size_t>(stacks * sectors * 6));

        const float sectorStep = constants::TwoPi / static_cast<float>(sectors);
        const float stackStep = constants::Pi / static_cast<float>(stacks);

        for (int i = 0; i <= stacks; ++i) {
            const float stackAngle = constants::HalfPi - static_cast<float>(i) * stackStep;
            const float xy = radius * std::cos(stackAngle);
            const float z = radius * std::sin(stackAngle);

            for (int j = 0; j <= sectors; ++j) {
                const float sectorAngle = static_cast<float>(j) * sectorStep;

                const float x = xy * std::cos(sectorAngle);
                const float y = xy * std::sin(sectorAngle);

                const Vec3 pos{x, z, y};
                const Vec3 normal = pos.normalized();
                const Vec2 texCoord{
                    static_cast<float>(j) / static_cast<float>(sectors),
                    static_cast<float>(i) / static_cast<float>(stacks)
                };

                data.vertices.push_back({pos, normal, texCoord});
            }
        }

        for (int i = 0; i < stacks; ++i) {
            std::uint32_t k1 = static_cast<std::uint32_t>(i * (sectors + 1));
            std::uint32_t k2 = k1 + static_cast<std::uint32_t>(sectors + 1);

            for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
                if (i != 0) {
                    data.indices.push_back(k1);
                    data.indices.push_back(k2);
                    data.indices.push_back(k1 + 1);
                }

                if (i != (stacks - 1)) {
                    data.indices.push_back(k1 + 1);
                    data.indices.push_back(k2);
                    data.indices.push_back(k2 + 1);
                }
            }
        }

        return data;
    }
};

} // namespace cad
