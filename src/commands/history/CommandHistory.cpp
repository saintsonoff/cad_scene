/// @file    CommandHistory.cpp
/// @brief   Implementation of CommandHistory.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#include "CommandHistory.hpp"

namespace cad {

CommandHistory::CommandHistory(std::size_t maxSize, QObject* parent)
    : QObject(parent)
    , m_maxSize(maxSize) {}

VoidResult CommandHistory::execute(CommandPtr command) {
    if (!command) {
        return std::unexpected(Error::make(ErrorCode::InvalidCommand, "Null command"));
    }

    if (!command->isValid()) {
        return std::unexpected(Error::make(ErrorCode::InvalidCommand, "Command is not valid"));
    }

    command->execute();
    QString desc = command->description();

    m_redoStack.clear();
    m_undoStack.push_back(std::move(command));
    trimToSize();

    emit commandExecuted(desc);
    emit historyChanged();

    return {};
}

VoidResult CommandHistory::undo() {
    if (m_undoStack.empty()) {
        return std::unexpected(Error::make(ErrorCode::NothingToUndo, "Nothing to undo"));
    }

    auto command = std::move(m_undoStack.back());
    m_undoStack.pop_back();

    if (!command->isValid()) {
        emit historyChanged();
        return std::unexpected(Error::make(ErrorCode::InvalidCommand, "Command target no longer exists"));
    }

    command->undo();
    QString desc = command->description();

    m_redoStack.push_back(std::move(command));

    emit commandUndone(desc);
    emit historyChanged();

    return {};
}

VoidResult CommandHistory::redo() {
    if (m_redoStack.empty()) {
        return std::unexpected(Error::make(ErrorCode::NothingToRedo, "Nothing to redo"));
    }

    auto command = std::move(m_redoStack.back());
    m_redoStack.pop_back();

    if (!command->isValid()) {
        emit historyChanged();
        return std::unexpected(Error::make(ErrorCode::InvalidCommand, "Command target no longer exists"));
    }

    command->execute();
    QString desc = command->description();

    m_undoStack.push_back(std::move(command));

    emit commandRedone(desc);
    emit historyChanged();

    return {};
}

bool CommandHistory::canUndo() const {
    return !m_undoStack.empty();
}

bool CommandHistory::canRedo() const {
    return !m_redoStack.empty();
}

QString CommandHistory::undoDescription() const {
    if (m_undoStack.empty()) {
        return {};
    }
    return m_undoStack.back()->description();
}

QString CommandHistory::redoDescription() const {
    if (m_redoStack.empty()) {
        return {};
    }
    return m_redoStack.back()->description();
}

std::size_t CommandHistory::undoCount() const {
    return m_undoStack.size();
}

std::size_t CommandHistory::redoCount() const {
    return m_redoStack.size();
}

void CommandHistory::clear() {
    m_undoStack.clear();
    m_redoStack.clear();
    emit historyChanged();
}

void CommandHistory::trimToSize() {
    while (m_undoStack.size() > m_maxSize) {
        m_undoStack.pop_front();
    }
}

} // namespace cad
