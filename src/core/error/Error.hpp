/// @file    Error.hpp
/// @brief   Error handling using std::expected.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include <QString>
#include <expected>

namespace cad {

/// @brief Categorized error codes for the application.
enum class ErrorCode {
    Success = 0,
    FileNotFound,
    FileReadError,
    FileWriteError,
    InvalidFormat,
    ShaderCompilationFailed,
    ShaderLinkFailed,
    OpenGLError,
    ObjectNotFound,
    InvalidObjectId,
    InvalidGeometry,
    CommandFailed,
    InvalidCommand,
    NothingToUndo,
    NothingToRedo,
    InvalidArgument,
    OutOfMemory,
    Unknown
};

/// @brief Error structure containing code and human-readable message.
struct Error {
    ErrorCode code{ErrorCode::Unknown};
    QString message;

    /// @brief Formats the error as a displayable string.
    [[nodiscard]] QString toString() const {
        return QString("[Error %1] %2").arg(static_cast<int>(code)).arg(message);
    }

    /// @brief Creates an error with the given code and message.
    static Error make(ErrorCode code, const QString& msg = {}) {
        return Error{code, msg};
    }

    /// @brief Creates a file-not-found error.
    static Error fileNotFound(const QString& path) {
        return {ErrorCode::FileNotFound, QString("File not found: %1").arg(path)};
    }

    /// @brief Creates an invalid-format error.
    static Error invalidFormat(const QString& details) {
        return {ErrorCode::InvalidFormat, QString("Invalid format: %1").arg(details)};
    }

    /// @brief Creates a shader compilation error.
    static Error shaderError(const QString& details) {
        return {ErrorCode::ShaderCompilationFailed, details};
    }

    /// @brief Creates an object-not-found error.
    static Error objectNotFound(std::uint64_t id) {
        return {ErrorCode::ObjectNotFound, QString("Object not found: %1").arg(id)};
    }
};

template <typename T>
using Result = std::expected<T, Error>;

using VoidResult = std::expected<void, Error>;

} // namespace cad
