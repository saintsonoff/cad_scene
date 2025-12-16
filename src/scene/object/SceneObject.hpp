/// @file    SceneObject.hpp
/// @brief   Renderable scene object with transform, material, and state.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include "IGeometry.hpp"
#include "Types.hpp"
#include <atomic>
#include <memory>
#include <QString>

namespace cad {

/// @brief A renderable 3D object in the scene graph.
/// @note Thread Safety: Not thread-safe. ID generation uses atomic operations.
class SceneObject {
public:
    /// @brief Constructs a scene object with geometry and optional name.
    /// @param geometry The mesh data to render. Must not be null.
    /// @param name Display name. Auto-generated if empty.
    explicit SceneObject(std::shared_ptr<IGeometry> geometry, QString name = {});

    [[nodiscard]] ObjectId id() const noexcept { return m_id; }
    [[nodiscard]] const QString& name() const { return m_name; }
    void setName(QString name) { m_name = std::move(name); }

    /// @brief Returns the geometry data for rendering.
    /// @return Reference to vertex/index data. Empty if geometry is null.
    [[nodiscard]] const GeometryData& geometry() const;

    /// @brief Computes world-space bounding box.
    /// @return AABB transformed by the model matrix.
    [[nodiscard]] AABB boundingBox() const;

    [[nodiscard]] const Transform& transform() const noexcept { return m_transform; }
    void setTransform(const Transform& transform) { m_transform = transform; }

    [[nodiscard]] Vec3 position() const noexcept { return m_transform.position; }
    void setPosition(const Vec3& pos) { m_transform.position = pos; }

    [[nodiscard]] Quat rotation() const noexcept { return m_transform.rotation; }
    void setRotation(const Quat& rot) { m_transform.rotation = rot; }

    [[nodiscard]] Vec3 scale() const noexcept { return m_transform.scale; }
    void setScale(const Vec3& s) { m_transform.scale = s; }

    [[nodiscard]] Mat4 modelMatrix() const noexcept { return m_transform.toMatrix(); }

    [[nodiscard]] const Material& material() const noexcept { return m_material; }
    void setMaterial(const Material& material) { m_material = material; }

    [[nodiscard]] Color baseColor() const noexcept { return m_material.baseColor; }
    void setBaseColor(const Color& color) { m_material.baseColor = color; }

    [[nodiscard]] bool isVisible() const noexcept { return m_visible; }
    void setVisible(bool visible) { m_visible = visible; }

    [[nodiscard]] bool isSelected() const noexcept { return m_selected; }
    void setSelected(bool selected) { m_selected = selected; }

private:
    static ObjectId generateId();

    ObjectId m_id;
    QString m_name;
    std::shared_ptr<IGeometry> m_geometry;
    Transform m_transform;
    Material m_material;
    bool m_visible{true};
    bool m_selected{false};
};

} // namespace cad
