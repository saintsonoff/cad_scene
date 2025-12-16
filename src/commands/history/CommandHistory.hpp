/// @file    CommandHistory.hpp
/// @brief   Undo/redo stack manager.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include "Error.hpp"
#include "ICommand.hpp"
#include <QObject>
#include <deque>
#include <cstddef>

namespace cad {

/// @brief Manages command history for undo/redo operations.
/// @note Thread Safety: Not thread-safe. Must be accessed from UI thread.
class CommandHistory : public QObject {
    Q_OBJECT

public:
    /// @brief Constructs a command history with a maximum stack size.
    /// @param maxSize Maximum number of commands to keep.
    /// @param parent Optional QObject parent.
    explicit CommandHistory(std::size_t maxSize = 100, QObject* parent = nullptr);
    ~CommandHistory() override = default;

    /// @brief Executes a command and adds it to the undo stack.
    /// @param command The command to execute. Takes ownership.
    /// @return Success or error if command is null/invalid.
    VoidResult execute(CommandPtr command);

    /// @brief Undoes the most recent command.
    VoidResult undo();

    /// @brief Redoes the most recently undone command.
    VoidResult redo();

    [[nodiscard]] bool canUndo() const;
    [[nodiscard]] bool canRedo() const;

    [[nodiscard]] QString undoDescription() const;
    [[nodiscard]] QString redoDescription() const;

    [[nodiscard]] std::size_t undoCount() const;
    [[nodiscard]] std::size_t redoCount() const;

    void clear();

signals:
    void historyChanged();
    void commandExecuted(const QString& description);
    void commandUndone(const QString& description);
    void commandRedone(const QString& description);

private:
    void trimToSize();

    std::deque<CommandPtr> m_undoStack;
    std::deque<CommandPtr> m_redoStack;
    std::size_t m_maxSize;
};

} // namespace cad
