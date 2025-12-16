/// @file    Scene.hpp
/// @brief   Scene graph management with selection and signals.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include "SceneObject.hpp"
#include <QObject>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

namespace cad {

/// @brief Manages a collection of 3D objects with selection and signals.
/// @note Thread Safety: Not thread-safe. Must be accessed from UI thread only.
class Scene : public QObject {
    Q_OBJECT

public:
    explicit Scene(QObject* parent = nullptr);
    ~Scene() override = default;

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&&) = delete;
    Scene& operator=(Scene&&) = delete;

    /// @brief Adds an object to the scene.
    /// @param object The object to add. Must not be null.
    /// @return The object's ID, or InvalidObjectId if null.
    ObjectId addObject(std::shared_ptr<SceneObject> object);

    /// @brief Removes an object by ID.
    /// @param id The object identifier.
    /// @return True if removed, false if not found.
    bool removeObject(ObjectId id);

    /// @brief Removes all objects from the scene.
    void clear();

    /// @brief Finds an object by ID.
    /// @param id The object identifier.
    /// @return Shared pointer to object, or nullptr if not found.
    [[nodiscard]] std::shared_ptr<SceneObject> findObject(ObjectId id);
    [[nodiscard]] std::shared_ptr<const SceneObject> findObject(ObjectId id) const;

    [[nodiscard]] const std::vector<std::shared_ptr<SceneObject>>& objects() const;
    [[nodiscard]] std::vector<std::shared_ptr<SceneObject>> visibleObjects() const;

    [[nodiscard]] std::shared_ptr<SceneObject> selectedObject() const;
    [[nodiscard]] ObjectId selectedObjectId() const;
    void selectObject(ObjectId id);
    void clearSelection();

    /// @brief Performs ray-AABB intersection against visible objects.
    /// @param ray The picking ray in world space.
    /// @return ID of closest hit object, or nullopt if none.
    [[nodiscard]] std::optional<ObjectId> raycast(const Ray& ray) const;

    /// @brief Computes bounding box enclosing all visible objects.
    [[nodiscard]] AABB boundingBox() const;

signals:
    void objectAdded(ObjectId id);
    void objectRemoved(ObjectId id);
    void selectionChanged(ObjectId newSelection);
    void sceneCleared();

private:
    std::vector<std::shared_ptr<SceneObject>> m_objects;
    std::unordered_map<ObjectId, std::shared_ptr<SceneObject>> m_objectIndex;
    ObjectId m_selectedId{InvalidObjectId};
};

} // namespace cad
