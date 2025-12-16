/// @file    MeshCache.hpp
/// @brief   GPU buffer cache per ObjectId.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include "GlBuffer.hpp"
#include "Types.hpp"
#include <QOpenGLFunctions_4_1_Core>
#include <unordered_map>

namespace cad {

/// @brief Caches GPU buffers by ObjectId.
/// @note Call cleanup() while OpenGL context is current before destruction.
class MeshCache {
public:
    MeshCache() = default;
    ~MeshCache() = default;

    void setGlFunctions(QOpenGLFunctions_4_1_Core* gl) {
        m_gl = gl;
    }

    /// @brief Get existing buffer or create new one from geometry data.
    GlBuffer* getOrCreate(ObjectId id, const GeometryData& data) {
        auto it = m_cache.find(id);
        if (it != m_cache.end()) {
            return &it->second;
        }

        if (m_gl && !data.isEmpty()) {
            auto& buffer = m_cache[id];
            buffer.create(m_gl, data);
            return &buffer;
        }

        return nullptr;
    }

    /// @brief Remove buffer for object. Must call while context is current.
    void invalidate(ObjectId id) {
        auto it = m_cache.find(id);
        if (it != m_cache.end()) {
            it->second.cleanup();
            m_cache.erase(it);
        }
    }

    /// @brief Release all buffers. Must call while context is current.
    void cleanup() {
        for (auto& [id, buffer] : m_cache) {
            buffer.cleanup();
        }
        m_cache.clear();
    }

    [[nodiscard]] bool contains(ObjectId id) const {
        return m_cache.contains(id);
    }

private:
    QOpenGLFunctions_4_1_Core* m_gl{nullptr};
    std::unordered_map<ObjectId, GlBuffer> m_cache;
};

} // namespace cad
