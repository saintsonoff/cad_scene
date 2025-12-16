/// @file    ICommand.hpp
/// @brief   Command interface for undo/redo operations.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include <QString>
#include <memory>

namespace cad {

/// @brief Abstract command following the Command + Memento pattern.
/// @note Commands store previous state at execute() time and use ObjectId references.
class ICommand {
public:
    virtual ~ICommand() = default;

    /// @brief Executes the command and stores previous state.
    virtual void execute() = 0;

    /// @brief Restores the previous state.
    virtual void undo() = 0;

    /// @brief Checks if the command target still exists.
    [[nodiscard]] virtual bool isValid() const = 0;

    /// @brief Returns a human-readable description for the UI.
    [[nodiscard]] virtual QString description() const = 0;

protected:
    ICommand() = default;
    ICommand(const ICommand&) = default;
    ICommand(ICommand&&) = default;
    ICommand& operator=(const ICommand&) = default;
    ICommand& operator=(ICommand&&) = default;
};

using CommandPtr = std::unique_ptr<ICommand>;

} // namespace cad
