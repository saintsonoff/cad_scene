/// @file    TransformCommands.hpp
/// @brief   Transform commands with memento pattern for undo/redo.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include "ICommand.hpp"
#include "Scene.hpp"
#include "Types.hpp"

namespace cad {

/// @brief Command to move an object to a new position.
class MoveCommand final : public ICommand {
public:
    MoveCommand(Scene& scene, ObjectId objectId, const Vec3& newPosition);

    void execute() override;
    void undo() override;
    [[nodiscard]] bool isValid() const override;
    [[nodiscard]] QString description() const override;

private:
    Scene& m_scene;
    ObjectId m_objectId;
    Vec3 m_newPosition;
    Vec3 m_previousPosition;
    bool m_executed{false};
};

/// @brief Command to scale an object.
class ScaleCommand final : public ICommand {
public:
    ScaleCommand(Scene& scene, ObjectId objectId, const Vec3& newScale);

    void execute() override;
    void undo() override;
    [[nodiscard]] bool isValid() const override;
    [[nodiscard]] QString description() const override;

private:
    Scene& m_scene;
    ObjectId m_objectId;
    Vec3 m_newScale;
    Vec3 m_previousScale;
    bool m_executed{false};
};

/// @brief Command to rotate an object using quaternion.
class RotateCommand final : public ICommand {
public:
    RotateCommand(Scene& scene, ObjectId objectId, const Quat& newRotation);

    void execute() override;
    void undo() override;
    [[nodiscard]] bool isValid() const override;
    [[nodiscard]] QString description() const override;

private:
    Scene& m_scene;
    ObjectId m_objectId;
    Quat m_newRotation;
    Quat m_previousRotation;
    bool m_executed{false};
};

/// @brief Command to toggle object visibility.
class SetVisibilityCommand final : public ICommand {
public:
    SetVisibilityCommand(Scene& scene, ObjectId objectId, bool visible);

    void execute() override;
    void undo() override;
    [[nodiscard]] bool isValid() const override;
    [[nodiscard]] QString description() const override;

private:
    Scene& m_scene;
    ObjectId m_objectId;
    bool m_newVisibility;
    bool m_previousVisibility;  // Memento: stored at execute()
    bool m_executed{false};
};

} // namespace cad
