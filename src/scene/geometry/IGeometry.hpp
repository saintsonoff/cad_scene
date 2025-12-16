/// @file    IGeometry.hpp
/// @brief   Abstract interface for geometry providers.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include "Types.hpp"

namespace cad {

/// @brief Abstract base class for geometry data sources.
class IGeometry {
public:
    virtual ~IGeometry() = default;

    /// @brief Returns the vertex and index data.
    [[nodiscard]] virtual const GeometryData& geometry() const = 0;

    /// @brief Computes the local-space bounding box.
    [[nodiscard]] virtual AABB boundingBox() const = 0;

protected:
    IGeometry() = default;
    IGeometry(const IGeometry&) = default;
    IGeometry(IGeometry&&) = default;
    IGeometry& operator=(const IGeometry&) = default;
    IGeometry& operator=(IGeometry&&) = default;
};

} // namespace cad
