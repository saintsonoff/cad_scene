/// @file    Scene.cpp
/// @brief   Implementation of Scene.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#include "Scene.hpp"
#include <algorithm>
#include <cmath>
#include <limits>

namespace cad {

Scene::Scene(QObject* parent)
    : QObject(parent) {}

ObjectId Scene::addObject(std::shared_ptr<SceneObject> object) {
    if (!object) {
        return InvalidObjectId;
    }

    ObjectId id = object->id();
    m_objects.push_back(object);
    m_objectIndex[id] = std::move(object);

    emit objectAdded(id);
    return id;
}

bool Scene::removeObject(ObjectId id) {
    auto indexIt = m_objectIndex.find(id);
    if (indexIt == m_objectIndex.end()) {
        return false;
    }

    auto it = std::ranges::find_if(m_objects, [id](const auto& obj) {
        return obj->id() == id;
    });

    if (it != m_objects.end()) {
        m_objects.erase(it);
    }

    m_objectIndex.erase(indexIt);

    if (m_selectedId == id) {
        clearSelection();
    }

    emit objectRemoved(id);
    return true;
}

void Scene::clear() {
    m_objects.clear();
    m_objectIndex.clear();
    m_selectedId = InvalidObjectId;
    emit sceneCleared();
}

std::shared_ptr<SceneObject> Scene::findObject(ObjectId id) {
    auto it = m_objectIndex.find(id);
    return (it != m_objectIndex.end()) ? it->second : nullptr;
}

std::shared_ptr<const SceneObject> Scene::findObject(ObjectId id) const {
    auto it = m_objectIndex.find(id);
    return (it != m_objectIndex.end()) ? it->second : nullptr;
}

const std::vector<std::shared_ptr<SceneObject>>& Scene::objects() const {
    return m_objects;
}

std::vector<std::shared_ptr<SceneObject>> Scene::visibleObjects() const {
    std::vector<std::shared_ptr<SceneObject>> visible;
    visible.reserve(m_objects.size());

    for (const auto& obj : m_objects) {
        if (obj->isVisible()) {
            visible.push_back(obj);
        }
    }

    return visible;
}

std::shared_ptr<SceneObject> Scene::selectedObject() const {
    if (m_selectedId == InvalidObjectId) {
        return nullptr;
    }

    auto it = std::ranges::find_if(m_objects, [this](const auto& obj) {
        return obj->id() == m_selectedId;
    });

    return (it != m_objects.end()) ? *it : nullptr;
}

ObjectId Scene::selectedObjectId() const {
    return m_selectedId;
}

void Scene::selectObject(ObjectId id) {
    // Deselect previous
    if (auto prev = findObject(m_selectedId)) {
        prev->setSelected(false);
    }

    m_selectedId = id;

    // Select new
    if (auto obj = findObject(id)) {
        obj->setSelected(true);
    }

    emit selectionChanged(m_selectedId);
}

void Scene::clearSelection() {
    if (auto obj = findObject(m_selectedId)) {
        obj->setSelected(false);
    }
    m_selectedId = InvalidObjectId;
    emit selectionChanged(InvalidObjectId);
}

std::optional<ObjectId> Scene::raycast(const Ray& ray) const {
    float closestT = std::numeric_limits<float>::max();
    ObjectId closestId = InvalidObjectId;

    // Precompute inverse direction to avoid division in inner loop.
    constexpr float epsilon = 1e-8f;
    auto safeInverse = [](float val) -> float {
        return std::abs(val) > epsilon ? 1.0f / val : std::copysign(1e8f, val);
    };

    const Vec3 invDir(
        safeInverse(ray.direction.x()),
        safeInverse(ray.direction.y()),
        safeInverse(ray.direction.z())
    );

    for (const auto& obj : m_objects) {
        if (!obj->isVisible()) {
            continue;
        }

        AABB bbox = obj->boundingBox();
        if (!bbox.isValid()) {
            continue;
        }

        // Ray-AABB intersection test using precomputed inverse direction
        float tmin = (bbox.min.x() - ray.origin.x()) * invDir.x();
        float tmax = (bbox.max.x() - ray.origin.x()) * invDir.x();

        if (tmin > tmax) std::swap(tmin, tmax);

        float tymin = (bbox.min.y() - ray.origin.y()) * invDir.y();
        float tymax = (bbox.max.y() - ray.origin.y()) * invDir.y();

        if (tymin > tymax) std::swap(tymin, tymax);

        if ((tmin > tymax) || (tymin > tmax)) {
            continue;
        }

        tmin = std::max(tmin, tymin);
        tmax = std::min(tmax, tymax);

        float tzmin = (bbox.min.z() - ray.origin.z()) * invDir.z();
        float tzmax = (bbox.max.z() - ray.origin.z()) * invDir.z();

        if (tzmin > tzmax) std::swap(tzmin, tzmax);

        if ((tmin > tzmax) || (tzmin > tmax)) {
            continue;
        }

        tmin = std::max(tmin, tzmin);

        if (tmin > 0.0f && tmin < closestT) {
            closestT = tmin;
            closestId = obj->id();
        }
    }

    if (closestId != InvalidObjectId) {
        return closestId;
    }
    return std::nullopt;
}

AABB Scene::boundingBox() const {
    AABB sceneBbox;

    for (const auto& obj : m_objects) {
        if (obj->isVisible()) {
            AABB objBbox = obj->boundingBox();
            if (objBbox.isValid()) {
                sceneBbox.expand(objBbox.min);
                sceneBbox.expand(objBbox.max);
            }
        }
    }

    return sceneBbox;
}

} // namespace cad
