/// @file    FileLoaderRegistry.cpp
/// @brief   Implementation of file loader registry.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#include "FileLoaderRegistry.hpp"
#include "StlLoader.hpp"

namespace cad {

FileLoaderRegistry::FileLoaderRegistry() {
    registerLoader(std::make_unique<StlLoader>());
}

void FileLoaderRegistry::registerLoader(std::unique_ptr<IFileLoader> loader) {
    if (loader) {
        m_loaders.push_back(std::move(loader));
    }
}

Result<GeometryData> FileLoaderRegistry::load(const QString& filePath) const {
    for (const auto& loader : m_loaders) {
        if (loader->canLoad(filePath)) {
            return loader->load(filePath);
        }
    }

    return std::unexpected(Error::make(ErrorCode::InvalidFormat,
        QString("No loader found for: %1").arg(filePath)));
}

QStringList FileLoaderRegistry::supportedExtensions() const {
    QStringList extensions;
    for (const auto& loader : m_loaders) {
        extensions.append(loader->supportedExtensions());
    }
    extensions.removeDuplicates();
    return extensions;
}

QString FileLoaderRegistry::fileFilter() const {
    QStringList filters;
    for (const auto& ext : supportedExtensions()) {
        filters.append(QString("*.%1").arg(ext));
    }
    return QString("3D Models (%1)").arg(filters.join(' '));
}

} // namespace cad
