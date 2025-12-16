/// @file    MaterialCommand.cpp
/// @brief   Implementation of material commands.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#include "MaterialCommand.hpp"

namespace cad {

SetColorCommand::SetColorCommand(Scene& scene, ObjectId objectId, const Color& newColor)
    : m_scene(scene)
    , m_objectId(objectId)
    , m_newColor(newColor) {}

void SetColorCommand::execute() {
    if (auto obj = m_scene.findObject(m_objectId)) {
        if (!m_executed) {
            m_previousColor = obj->baseColor();
            m_executed = true;
        }
        obj->setBaseColor(m_newColor);
    }
}

void SetColorCommand::undo() {
    if (m_executed) {
        if (auto obj = m_scene.findObject(m_objectId)) {
            obj->setBaseColor(m_previousColor);
        }
    }
}

bool SetColorCommand::isValid() const {
    return m_scene.findObject(m_objectId) != nullptr;
}

QString SetColorCommand::description() const {
    return QString("Set color of object %1 to %2")
        .arg(m_objectId)
        .arg(m_newColor.name());
}

SetMaterialCommand::SetMaterialCommand(Scene& scene, ObjectId objectId, const Material& newMaterial)
    : m_scene(scene)
    , m_objectId(objectId)
    , m_newMaterial(newMaterial) {}

void SetMaterialCommand::execute() {
    if (auto obj = m_scene.findObject(m_objectId)) {
        if (!m_executed) {
            m_previousMaterial = obj->material();
            m_executed = true;
        }
        obj->setMaterial(m_newMaterial);
    }
}

void SetMaterialCommand::undo() {
    if (m_executed) {
        if (auto obj = m_scene.findObject(m_objectId)) {
            obj->setMaterial(m_previousMaterial);
        }
    }
}

bool SetMaterialCommand::isValid() const {
    return m_scene.findObject(m_objectId) != nullptr;
}

QString SetMaterialCommand::description() const {
    return QString("Set material of object %1").arg(m_objectId);
}

} // namespace cad
