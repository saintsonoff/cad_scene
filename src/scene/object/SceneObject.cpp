/// @file    SceneObject.cpp
/// @brief   Implementation of SceneObject.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#include "SceneObject.hpp"

namespace cad {

namespace {
std::atomic<ObjectId> s_nextId{1};
} // namespace

ObjectId SceneObject::generateId() {
    return s_nextId.fetch_add(1, std::memory_order_relaxed);
}

SceneObject::SceneObject(std::shared_ptr<IGeometry> geometry, QString name)
    : m_id(generateId())
    , m_name(std::move(name))
    , m_geometry(std::move(geometry)) {
    if (m_name.isEmpty()) {
        m_name = QString("Object_%1").arg(m_id);
    }
}

const GeometryData& SceneObject::geometry() const {
    static const GeometryData empty;
    return m_geometry ? m_geometry->geometry() : empty;
}

AABB SceneObject::boundingBox() const {
    if (!m_geometry) {
        return {};
    }

    AABB localBbox = m_geometry->boundingBox();
    Mat4 model = modelMatrix();

    AABB worldBbox;
    const Vec3 corners[8] = {
        {localBbox.min.x(), localBbox.min.y(), localBbox.min.z()},
        {localBbox.max.x(), localBbox.min.y(), localBbox.min.z()},
        {localBbox.min.x(), localBbox.max.y(), localBbox.min.z()},
        {localBbox.max.x(), localBbox.max.y(), localBbox.min.z()},
        {localBbox.min.x(), localBbox.min.y(), localBbox.max.z()},
        {localBbox.max.x(), localBbox.min.y(), localBbox.max.z()},
        {localBbox.min.x(), localBbox.max.y(), localBbox.max.z()},
        {localBbox.max.x(), localBbox.max.y(), localBbox.max.z()}
    };

    for (const auto& corner : corners) {
        Vec3 transformed = model.map(corner);
        worldBbox.expand(transformed);
    }

    return worldBbox;
}

} // namespace cad
