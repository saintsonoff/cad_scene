/// @file    CubeGeometry.hpp
/// @brief   Cube primitive generator.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include "MeshGeometry.hpp"

namespace cad {

/// @brief Static factory for cube mesh generation.
class CubeGeometry {
public:
    /// @brief Generates a unit cube centered at origin.
    /// @param size The edge length of the cube.
    /// @return GeometryData with 24 vertices and 36 indices.
    [[nodiscard]] static GeometryData generate(float size = 1.0f) {
        const float h = size * 0.5f;

        GeometryData data;
        data.vertices.reserve(24);
        data.indices.reserve(36);

        // Face normals
        const Vec3 normals[] = {
            { 0.0f,  0.0f,  1.0f},  // Front
            { 0.0f,  0.0f, -1.0f},  // Back
            { 1.0f,  0.0f,  0.0f},  // Right
            {-1.0f,  0.0f,  0.0f},  // Left
            { 0.0f,  1.0f,  0.0f},  // Top
            { 0.0f, -1.0f,  0.0f}   // Bottom
        };

        // Face vertices (4 per face, 6 faces)
        const Vec3 faceVertices[6][4] = {
            // Front face
            {{-h, -h,  h}, { h, -h,  h}, { h,  h,  h}, {-h,  h,  h}},
            // Back face
            {{ h, -h, -h}, {-h, -h, -h}, {-h,  h, -h}, { h,  h, -h}},
            // Right face
            {{ h, -h,  h}, { h, -h, -h}, { h,  h, -h}, { h,  h,  h}},
            // Left face
            {{-h, -h, -h}, {-h, -h,  h}, {-h,  h,  h}, {-h,  h, -h}},
            // Top face
            {{-h,  h,  h}, { h,  h,  h}, { h,  h, -h}, {-h,  h, -h}},
            // Bottom face
            {{-h, -h, -h}, { h, -h, -h}, { h, -h,  h}, {-h, -h,  h}}
        };

        const Vec2 texCoords[] = {
            {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}
        };

        for (int face = 0; face < 6; ++face) {
            const std::uint32_t baseIndex = static_cast<std::uint32_t>(data.vertices.size());

            for (int v = 0; v < 4; ++v) {
                data.vertices.push_back({
                    faceVertices[face][v],
                    normals[face],
                    texCoords[v]
                });
            }

            // Two triangles per face
            data.indices.push_back(baseIndex + 0);
            data.indices.push_back(baseIndex + 1);
            data.indices.push_back(baseIndex + 2);
            data.indices.push_back(baseIndex + 0);
            data.indices.push_back(baseIndex + 2);
            data.indices.push_back(baseIndex + 3);
        }

        return data;
    }
};

} // namespace cad
