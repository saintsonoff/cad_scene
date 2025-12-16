/// @file    StlLoader.hpp
/// @brief   STL file loader (ASCII and Binary).
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include "IFileLoader.hpp"

namespace cad {

/// @brief Loads STL files in both ASCII and binary formats.
class StlLoader final : public IFileLoader {
public:
    [[nodiscard]] QStringList supportedExtensions() const override;
    [[nodiscard]] bool canLoad(const QString& filePath) const override;
    [[nodiscard]] Result<GeometryData> load(const QString& filePath) const override;

private:
    [[nodiscard]] Result<GeometryData> loadAscii(const QString& filePath) const;
    [[nodiscard]] Result<GeometryData> loadBinary(const QString& filePath) const;
    [[nodiscard]] bool isBinaryStl(const QString& filePath) const;
};

} // namespace cad
