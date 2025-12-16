/// @file    MaterialCommand.hpp
/// @brief   Material/Color commands with memento pattern.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include "ICommand.hpp"
#include "Scene.hpp"
#include "Types.hpp"

namespace cad {

/// @brief Command to change an object's base color.
class SetColorCommand final : public ICommand {
public:
    SetColorCommand(Scene& scene, ObjectId objectId, const Color& newColor);

    void execute() override;
    void undo() override;

    [[nodiscard]] bool isValid() const override;
    [[nodiscard]] QString description() const override;

private:
    Scene& m_scene;
    ObjectId m_objectId;
    Color m_newColor;
    Color m_previousColor;
    bool m_executed{false};
};

/// @brief Command to change an object's entire material.
class SetMaterialCommand final : public ICommand {
public:
    SetMaterialCommand(Scene& scene, ObjectId objectId, const Material& newMaterial);

    void execute() override;
    void undo() override;

    [[nodiscard]] bool isValid() const override;
    [[nodiscard]] QString description() const override;

private:
    Scene& m_scene;
    ObjectId m_objectId;
    Material m_newMaterial;
    Material m_previousMaterial;
    bool m_executed{false};
};

} // namespace cad
