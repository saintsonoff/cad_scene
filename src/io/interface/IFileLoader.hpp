/// @file    IFileLoader.hpp
/// @brief   File loader interface.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include "Error.hpp"
#include "Types.hpp"
#include <QString>
#include <QStringList>

namespace cad {

/// @brief Interface for geometry file loaders.
class IFileLoader {
public:
    virtual ~IFileLoader() = default;

    /// @brief Get list of supported file extensions.
    [[nodiscard]] virtual QStringList supportedExtensions() const = 0;

    /// @brief Check if file can be loaded.
    [[nodiscard]] virtual bool canLoad(const QString& filePath) const = 0;

    /// @brief Load geometry from file.
    [[nodiscard]] virtual Result<GeometryData> load(const QString& filePath) const = 0;

protected:
    IFileLoader() = default;
    IFileLoader(const IFileLoader&) = default;
    IFileLoader(IFileLoader&&) = default;
    IFileLoader& operator=(const IFileLoader&) = default;
    IFileLoader& operator=(IFileLoader&&) = default;
};

} // namespace cad
