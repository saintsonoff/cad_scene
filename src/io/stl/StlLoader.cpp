/// @file    StlLoader.cpp
/// @brief   Implementation of STL file loader.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#include "StlLoader.hpp"
#include "Constants.hpp"
#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <cstring>

namespace cad {

QStringList StlLoader::supportedExtensions() const {
    return {"stl", "STL"};
}

bool StlLoader::canLoad(const QString& filePath) const {
    QFileInfo info(filePath);
    return supportedExtensions().contains(info.suffix(), Qt::CaseInsensitive);
}

Result<GeometryData> StlLoader::load(const QString& filePath) const {
    QFile file(filePath);
    if (!file.exists()) {
        return std::unexpected(Error::fileNotFound(filePath));
    }

    if (!file.open(QIODevice::ReadOnly)) {
        return std::unexpected(Error::make(ErrorCode::FileReadError,
            QString("Cannot open file: %1").arg(filePath)));
    }

    file.close();

    if (isBinaryStl(filePath)) {
        return loadBinary(filePath);
    }
    return loadAscii(filePath);
}

bool StlLoader::isBinaryStl(const QString& filePath) const {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray header = file.read(256);
    file.close();

    // Binary files may coincidentally start with "solid", check for ASCII keywords
    if (header.startsWith("solid")) {
        QByteArray content = header.mid(0, 256);
        if (content.contains("facet") || content.contains("vertex") || content.contains("endsolid")) {
            return false;
        }
    }

    return true;
}

Result<GeometryData> StlLoader::loadAscii(const QString& filePath) const {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return std::unexpected(Error::make(ErrorCode::FileReadError, "Cannot open ASCII STL"));
    }

    GeometryData data;
    QTextStream stream(&file);

    Vec3 currentNormal;
    std::vector<Vec3> faceVertices;
    faceVertices.reserve(3);

    while (!stream.atEnd()) {
        QString line = stream.readLine().trimmed();

        if (line.startsWith("facet normal")) {
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);
            if (parts.size() >= 5) {
                currentNormal = Vec3(
                    parts[2].toFloat(),
                    parts[3].toFloat(),
                    parts[4].toFloat()
                );
            }
            faceVertices.clear();
        }
        else if (line.startsWith("vertex")) {
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);
            if (parts.size() >= 4) {
                Vec3 pos(
                    parts[1].toFloat(),
                    parts[2].toFloat(),
                    parts[3].toFloat()
                );
                faceVertices.push_back(pos);
            }
        }
        else if (line.startsWith("endfacet")) {
            if (faceVertices.size() == 3) {
                std::uint32_t baseIndex = static_cast<std::uint32_t>(data.vertices.size());

                for (const auto& pos : faceVertices) {
                    data.vertices.push_back({pos, currentNormal, Vec2{0.0f, 0.0f}});
                }

                data.indices.push_back(baseIndex);
                data.indices.push_back(baseIndex + 1);
                data.indices.push_back(baseIndex + 2);
            }
        }
    }

    return data;
}

Result<GeometryData> StlLoader::loadBinary(const QString& filePath) const {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return std::unexpected(Error::make(ErrorCode::FileReadError, "Cannot open binary STL"));
    }

    const qint64 fileSize = file.size();

    constexpr qint64 minFileSize = static_cast<qint64>(constants::StlHeaderSize) + 4;
    if (fileSize < minFileSize) {
        return std::unexpected(Error::invalidFormat("File too small to be a valid STL"));
    }

    file.seek(constants::StlHeaderSize);

    std::uint32_t triangleCount = 0;
    if (file.read(reinterpret_cast<char*>(&triangleCount), sizeof(triangleCount)) != sizeof(triangleCount)) {
        return std::unexpected(Error::invalidFormat("Cannot read triangle count"));
    }

    const qint64 expectedSize = minFileSize + 
        static_cast<qint64>(triangleCount) * static_cast<qint64>(constants::StlTriangleSize);
    if (fileSize < expectedSize) {
        return std::unexpected(Error::invalidFormat(
            QString("File too small for declared triangle count (%1). Expected %2 bytes, got %3")
                .arg(triangleCount)
                .arg(expectedSize)
                .arg(fileSize)));
    }

    constexpr std::uint32_t maxTriangles = 100'000'000;
    if (triangleCount > maxTriangles) {
        return std::unexpected(Error::invalidFormat(
            QString("Triangle count too large (%1 > %2)")
                .arg(triangleCount)
                .arg(maxTriangles)));
    }

    GeometryData data;
    data.vertices.reserve(triangleCount * 3);
    data.indices.reserve(triangleCount * 3);

    for (std::uint32_t i = 0; i < triangleCount; ++i) {
        float normal[3];
        if (file.read(reinterpret_cast<char*>(normal), sizeof(normal)) != sizeof(normal)) {
            return std::unexpected(Error::invalidFormat("Unexpected end of file"));
        }
        Vec3 n(normal[0], normal[1], normal[2]);

        for (int v = 0; v < 3; ++v) {
            float pos[3];
            if (file.read(reinterpret_cast<char*>(pos), sizeof(pos)) != sizeof(pos)) {
                return std::unexpected(Error::invalidFormat("Unexpected end of file"));
            }

            std::uint32_t idx = static_cast<std::uint32_t>(data.vertices.size());
            data.vertices.push_back({Vec3(pos[0], pos[1], pos[2]), n, Vec2{0.0f, 0.0f}});
            data.indices.push_back(idx);
        }

        file.seek(file.pos() + 2);
    }

    return data;
}

} // namespace cad
