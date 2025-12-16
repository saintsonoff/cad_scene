/// @file    Types.hpp
/// @brief   Core type definitions using Qt Math exclusively.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include <QColor>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QString>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <cstdint>
#include <expected>
#include <optional>
#include <vector>

namespace cad {

using Vec2 = QVector2D;
using Vec3 = QVector3D;
using Vec4 = QVector4D;
using Mat4 = QMatrix4x4;
using Quat = QQuaternion;
using Color = QColor;

using ObjectId = std::uint64_t;
constexpr ObjectId InvalidObjectId = 0;

/// @brief Single vertex with position, normal, and texture coordinates.
struct Vertex {
    Vec3 position{0.0f, 0.0f, 0.0f};
    Vec3 normal{0.0f, 1.0f, 0.0f};
    Vec2 texCoord{0.0f, 0.0f};

    constexpr bool operator==(const Vertex&) const noexcept = default;
};

/// @brief Container for mesh vertex and index data.
struct GeometryData {
    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;

    [[nodiscard]] bool isEmpty() const noexcept { return vertices.empty(); }
    [[nodiscard]] std::size_t vertexCount() const noexcept { return vertices.size(); }
    [[nodiscard]] std::size_t indexCount() const noexcept { return indices.size(); }
};

/// @brief Axis-aligned bounding box for spatial queries.
struct AABB {
    Vec3 min{std::numeric_limits<float>::max(),
             std::numeric_limits<float>::max(),
             std::numeric_limits<float>::max()};
    Vec3 max{std::numeric_limits<float>::lowest(),
             std::numeric_limits<float>::lowest(),
             std::numeric_limits<float>::lowest()};

    [[nodiscard]] Vec3 center() const noexcept { return (min + max) * 0.5f; }
    [[nodiscard]] Vec3 size() const noexcept { return max - min; }
    [[nodiscard]] bool isValid() const noexcept {
        return min.x() <= max.x() && min.y() <= max.y() && min.z() <= max.z();
    }

    void expand(const Vec3& point) noexcept {
        min.setX(std::min(min.x(), point.x()));
        min.setY(std::min(min.y(), point.y()));
        min.setZ(std::min(min.z(), point.z()));
        max.setX(std::max(max.x(), point.x()));
        max.setY(std::max(max.y(), point.y()));
        max.setZ(std::max(max.z(), point.z()));
    }
};

/// @brief Ray for picking and intersection tests.
struct Ray {
    Vec3 origin{0.0f, 0.0f, 0.0f};
    Vec3 direction{0.0f, 0.0f, -1.0f};

    [[nodiscard]] Vec3 pointAt(float t) const noexcept { return origin + direction * t; }
};

/// @brief Position, rotation (quaternion), and scale for scene objects.
struct Transform {
    Vec3 position{0.0f, 0.0f, 0.0f};
    Quat rotation{};
    Vec3 scale{1.0f, 1.0f, 1.0f};

    /// @brief Computes the 4x4 model matrix from TRS components.
    [[nodiscard]] Mat4 toMatrix() const noexcept {
        Mat4 matrix;
        matrix.setToIdentity();
        matrix.translate(position);
        matrix.rotate(rotation);
        matrix.scale(scale);
        return matrix;
    }

    /// @brief Decomposes a 4x4 matrix into TRS components.
    /// @param matrix The transformation matrix to decompose.
    /// @return Transform with extracted position, rotation, and scale.
    static Transform fromMatrix(const Mat4& matrix) noexcept {
        Transform t;
        t.position = Vec3(matrix(0, 3), matrix(1, 3), matrix(2, 3));

        Vec3 col0(matrix(0, 0), matrix(1, 0), matrix(2, 0));
        Vec3 col1(matrix(0, 1), matrix(1, 1), matrix(2, 1));
        Vec3 col2(matrix(0, 2), matrix(1, 2), matrix(2, 2));
        t.scale = Vec3(col0.length(), col1.length(), col2.length());

        if (t.scale.x() > 0.0f) col0 /= t.scale.x();
        if (t.scale.y() > 0.0f) col1 /= t.scale.y();
        if (t.scale.z() > 0.0f) col2 /= t.scale.z();

        QMatrix3x3 rotMat;
        rotMat(0, 0) = col0.x(); rotMat(1, 0) = col0.y(); rotMat(2, 0) = col0.z();
        rotMat(0, 1) = col1.x(); rotMat(1, 1) = col1.y(); rotMat(2, 1) = col1.z();
        rotMat(0, 2) = col2.x(); rotMat(1, 2) = col2.y(); rotMat(2, 2) = col2.z();
        t.rotation = Quat::fromRotationMatrix(rotMat);

        return t;
    }
};

/// @brief PBR material properties for rendering.
struct Material {
    Color baseColor{180, 180, 190};
    float metallic{0.0f};
    float roughness{0.5f};
    float shininess{0.5f};  ///< Specular intensity: 0.0 = matte, 1.0 = glossy/mirror
    float ambientOcclusion{1.0f};

    [[nodiscard]] Vec3 baseColorVec3() const noexcept {
        return Vec3(baseColor.redF(), baseColor.greenF(), baseColor.blueF());
    }

    void setBaseColorFromVec3(const Vec3& color) noexcept {
        baseColor.setRgbF(color.x(), color.y(), color.z());
    }
};

} // namespace cad
