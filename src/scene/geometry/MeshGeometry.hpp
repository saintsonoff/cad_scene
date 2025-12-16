/// @file    MeshGeometry.hpp
/// @brief   Concrete geometry implementation.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include "IGeometry.hpp"
#include <utility>

namespace cad {

/// @brief Concrete geometry implementation storing mesh data.
class MeshGeometry final : public IGeometry {
public:
    MeshGeometry() = default;

    explicit MeshGeometry(GeometryData data) 
        : m_data(std::move(data))
        , m_bbox(computeBoundingBox()) {}

    [[nodiscard]] const GeometryData& geometry() const override {
        return m_data;
    }

    [[nodiscard]] AABB boundingBox() const override {
        return m_bbox;
    }

    void setGeometry(GeometryData data) {
        m_data = std::move(data);
        m_bbox = computeBoundingBox();
    }

private:
    [[nodiscard]] AABB computeBoundingBox() const {
        AABB bbox;
        for (const auto& vertex : m_data.vertices) {
            bbox.expand(vertex.position);
        }
        return bbox;
    }

    GeometryData m_data;
    AABB m_bbox;
};

} // namespace cad
