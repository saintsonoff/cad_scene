/// @file    GlBuffer.hpp
/// @brief   OpenGL buffer wrapper (VAO, VBO, IBO).
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include "Types.hpp"
#include <QOpenGLFunctions_4_1_Core>
#include <cstdint>

namespace cad {

/// @brief OpenGL buffer wrapper managing VAO, VBO, and IBO.
/// @note Call cleanup() explicitly while OpenGL context is current before destruction.
class GlBuffer {
public:
    GlBuffer() = default;
    
    ~GlBuffer() {
        Q_ASSERT(m_vao == 0 && m_vbo == 0 && m_ibo == 0 && 
                 "GlBuffer::cleanup() was not called before destruction");
    }

    GlBuffer(const GlBuffer&) = delete;
    GlBuffer& operator=(const GlBuffer&) = delete;

    GlBuffer(GlBuffer&& other) noexcept { swap(other); }
    GlBuffer& operator=(GlBuffer&& other) noexcept {
        if (this != &other) {
            cleanup();
            swap(other);
        }
        return *this;
    }

    /// @brief Release OpenGL resources. Must be called while context is current.
    void cleanup() noexcept {
        if (m_gl) {
            if (m_vao != 0) {
                m_gl->glDeleteVertexArrays(1, &m_vao);
                m_vao = 0;
            }
            if (m_vbo != 0) {
                m_gl->glDeleteBuffers(1, &m_vbo);
                m_vbo = 0;
            }
            if (m_ibo != 0) {
                m_gl->glDeleteBuffers(1, &m_ibo);
                m_ibo = 0;
            }
        }
        m_gl = nullptr;
        m_indexCount = 0;
    }

    /// @brief Create buffer from geometry data.
    void create(QOpenGLFunctions_4_1_Core* gl, const GeometryData& data) {
        cleanup();

        m_gl = gl;
        m_indexCount = static_cast<GLsizei>(data.indices.size());

        if (data.isEmpty()) {
            return;
        }

        gl->glGenVertexArrays(1, &m_vao);
        gl->glBindVertexArray(m_vao);

        gl->glGenBuffers(1, &m_vbo);
        gl->glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        gl->glBufferData(GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(data.vertices.size() * sizeof(Vertex)),
            data.vertices.data(),
            GL_STATIC_DRAW);

        gl->glGenBuffers(1, &m_ibo);
        gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        gl->glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(data.indices.size() * sizeof(std::uint32_t)),
            data.indices.data(),
            GL_STATIC_DRAW);

        // Position (location 0)
        gl->glEnableVertexAttribArray(0);
        gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
            reinterpret_cast<void*>(offsetof(Vertex, position)));

        // Normal (location 1)
        gl->glEnableVertexAttribArray(1);
        gl->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
            reinterpret_cast<void*>(offsetof(Vertex, normal)));

        // TexCoord (location 2)
        gl->glEnableVertexAttribArray(2);
        gl->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
            reinterpret_cast<void*>(offsetof(Vertex, texCoord)));

        gl->glBindVertexArray(0);
    }

    void bind() const {
        if (m_gl && m_vao != 0) {
            m_gl->glBindVertexArray(m_vao);
        }
    }

    void unbind() const {
        if (m_gl) {
            m_gl->glBindVertexArray(0);
        }
    }

    void draw() const {
        if (m_gl && m_vao != 0 && m_indexCount > 0) {
            bind();
            m_gl->glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
            unbind();
        }
    }

    [[nodiscard]] bool isValid() const noexcept { return m_vao != 0; }
    [[nodiscard]] GLsizei indexCount() const noexcept { return m_indexCount; }

private:
    void swap(GlBuffer& other) noexcept {
        std::swap(m_gl, other.m_gl);
        std::swap(m_vao, other.m_vao);
        std::swap(m_vbo, other.m_vbo);
        std::swap(m_ibo, other.m_ibo);
        std::swap(m_indexCount, other.m_indexCount);
    }

    QOpenGLFunctions_4_1_Core* m_gl{nullptr};
    GLuint m_vao{0};
    GLuint m_vbo{0};
    GLuint m_ibo{0};
    GLsizei m_indexCount{0};
};

} // namespace cad
