/// @file    FileLoaderRegistry.hpp
/// @brief   Registry for file loaders (Dependency Injection).
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include "Error.hpp"
#include "IFileLoader.hpp"
#include "Types.hpp"
#include <memory>
#include <vector>

namespace cad {

/// @brief Registry managing file loaders.
/// @note Instance must be explicitly passed - no singleton/global state.
class FileLoaderRegistry {
public:
    FileLoaderRegistry();

    /// @brief Register a new file loader.
    void registerLoader(std::unique_ptr<IFileLoader> loader);

    /// @brief Load geometry from file using appropriate loader.
    [[nodiscard]] Result<GeometryData> load(const QString& filePath) const;

    /// @brief Get all supported file extensions.
    [[nodiscard]] QStringList supportedExtensions() const;

    /// @brief Get file dialog filter string.
    [[nodiscard]] QString fileFilter() const;

private:
    std::vector<std::unique_ptr<IFileLoader>> m_loaders;
};

} // namespace cad
