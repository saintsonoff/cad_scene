/// @file    TransformCommands.cpp
/// @brief   Implementation of transform commands.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#include "TransformCommands.hpp"

namespace cad {

MoveCommand::MoveCommand(Scene& scene, ObjectId objectId, const Vec3& newPosition)
    : m_scene(scene)
    , m_objectId(objectId)
    , m_newPosition(newPosition) {}

void MoveCommand::execute() {
    if (auto obj = m_scene.findObject(m_objectId)) {
        if (!m_executed) {
            m_previousPosition = obj->position();
            m_executed = true;
        }
        obj->setPosition(m_newPosition);
    }
}

void MoveCommand::undo() {
    if (m_executed) {
        if (auto obj = m_scene.findObject(m_objectId)) {
            obj->setPosition(m_previousPosition);
        }
    }
}

bool MoveCommand::isValid() const {
    return m_scene.findObject(m_objectId) != nullptr;
}

QString MoveCommand::description() const {
    return QString("Move object %1").arg(m_objectId);
}

ScaleCommand::ScaleCommand(Scene& scene, ObjectId objectId, const Vec3& newScale)
    : m_scene(scene)
    , m_objectId(objectId)
    , m_newScale(newScale) {}

void ScaleCommand::execute() {
    if (auto obj = m_scene.findObject(m_objectId)) {
        if (!m_executed) {
            m_previousScale = obj->scale();
            m_executed = true;
        }
        obj->setScale(m_newScale);
    }
}

void ScaleCommand::undo() {
    if (m_executed) {
        if (auto obj = m_scene.findObject(m_objectId)) {
            obj->setScale(m_previousScale);
        }
    }
}

bool ScaleCommand::isValid() const {
    return m_scene.findObject(m_objectId) != nullptr;
}

QString ScaleCommand::description() const {
    return QString("Scale object %1").arg(m_objectId);
}

RotateCommand::RotateCommand(Scene& scene, ObjectId objectId, const Quat& newRotation)
    : m_scene(scene)
    , m_objectId(objectId)
    , m_newRotation(newRotation) {}

void RotateCommand::execute() {
    if (auto obj = m_scene.findObject(m_objectId)) {
        if (!m_executed) {
            m_previousRotation = obj->rotation();
            m_executed = true;
        }
        obj->setRotation(m_newRotation);
    }
}

void RotateCommand::undo() {
    if (m_executed) {
        if (auto obj = m_scene.findObject(m_objectId)) {
            obj->setRotation(m_previousRotation);
        }
    }
}

bool RotateCommand::isValid() const {
    return m_scene.findObject(m_objectId) != nullptr;
}

QString RotateCommand::description() const {
    return QString("Rotate object %1").arg(m_objectId);
}

SetVisibilityCommand::SetVisibilityCommand(Scene& scene, ObjectId objectId, bool visible)
    : m_scene(scene)
    , m_objectId(objectId)
    , m_newVisibility(visible) {}

void SetVisibilityCommand::execute() {
    if (auto obj = m_scene.findObject(m_objectId)) {
        if (!m_executed) {
            m_previousVisibility = obj->isVisible();
            m_executed = true;
        }
        obj->setVisible(m_newVisibility);
    }
}

void SetVisibilityCommand::undo() {
    if (m_executed) {
        if (auto obj = m_scene.findObject(m_objectId)) {
            obj->setVisible(m_previousVisibility);
        }
    }
}

bool SetVisibilityCommand::isValid() const {
    return m_scene.findObject(m_objectId) != nullptr;
}

QString SetVisibilityCommand::description() const {
    return QString("Set visibility of object %1 to %2")
        .arg(m_objectId)
        .arg(m_newVisibility ? "visible" : "hidden");
}

} // namespace cad
