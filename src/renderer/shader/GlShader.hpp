/// @file    GlShader.hpp
/// @brief   OpenGL shader program wrapper.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include "Error.hpp"
#include "Types.hpp"
#include <QOpenGLFunctions_4_1_Core>
#include <QString>

namespace cad {

/// @brief OpenGL shader program wrapper.
/// @note Call cleanup() explicitly while OpenGL context is current before destruction.
class GlShader {
public:
    GlShader() = default;
    
    ~GlShader() {
        Q_ASSERT(m_program == 0 && 
                 "GlShader::cleanup() was not called before destruction");
    }

    GlShader(const GlShader&) = delete;
    GlShader& operator=(const GlShader&) = delete;

    GlShader(GlShader&& other) noexcept { swap(other); }
    GlShader& operator=(GlShader&& other) noexcept {
        if (this != &other) {
            cleanup();
            swap(other);
        }
        return *this;
    }

    /// @brief Release OpenGL resources. Must be called while context is current.
    void cleanup() noexcept {
        if (m_gl && m_program != 0) {
            m_gl->glDeleteProgram(m_program);
        }
        m_gl = nullptr;
        m_program = 0;
    }

    /// @brief Compile and link shader program from sources.
    Result<void> compile(QOpenGLFunctions_4_1_Core* gl,
                         const char* vertexSource,
                         const char* fragmentSource) {
        cleanup();
        m_gl = gl;

        GLuint vertexShader = gl->glCreateShader(GL_VERTEX_SHADER);
        gl->glShaderSource(vertexShader, 1, &vertexSource, nullptr);
        gl->glCompileShader(vertexShader);

        GLint success = 0;
        gl->glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char log[512];
            gl->glGetShaderInfoLog(vertexShader, 512, nullptr, log);
            gl->glDeleteShader(vertexShader);
            return std::unexpected(Error::shaderError(QString("Vertex shader: %1").arg(log)));
        }

        GLuint fragmentShader = gl->glCreateShader(GL_FRAGMENT_SHADER);
        gl->glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
        gl->glCompileShader(fragmentShader);

        gl->glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char log[512];
            gl->glGetShaderInfoLog(fragmentShader, 512, nullptr, log);
            gl->glDeleteShader(vertexShader);
            gl->glDeleteShader(fragmentShader);
            return std::unexpected(Error::shaderError(QString("Fragment shader: %1").arg(log)));
        }

        m_program = gl->glCreateProgram();
        gl->glAttachShader(m_program, vertexShader);
        gl->glAttachShader(m_program, fragmentShader);
        gl->glLinkProgram(m_program);

        gl->glGetProgramiv(m_program, GL_LINK_STATUS, &success);
        if (!success) {
            char log[512];
            gl->glGetProgramInfoLog(m_program, 512, nullptr, log);
            gl->glDeleteShader(vertexShader);
            gl->glDeleteShader(fragmentShader);
            gl->glDeleteProgram(m_program);
            m_program = 0;
            return std::unexpected(Error::shaderError(QString("Program link: %1").arg(log)));
        }

        gl->glDeleteShader(vertexShader);
        gl->glDeleteShader(fragmentShader);

        return {};
    }

    void bind() const {
        if (m_gl && m_program != 0) {
            m_gl->glUseProgram(m_program);
        }
    }

    void unbind() const {
        if (m_gl) {
            m_gl->glUseProgram(0);
        }
    }

    [[nodiscard]] GLint uniformLocation(const char* name) const {
        if (!m_gl || m_program == 0) return -1;
        return m_gl->glGetUniformLocation(m_program, name);
    }

    void setUniform(GLint location, float value) const {
        if (m_gl && location >= 0) {
            m_gl->glUniform1f(location, value);
        }
    }

    void setUniform(GLint location, const Vec3& value) const {
        if (m_gl && location >= 0) {
            m_gl->glUniform3f(location, value.x(), value.y(), value.z());
        }
    }

    void setUniform(GLint location, const Mat4& value) const {
        if (m_gl && location >= 0) {
            m_gl->glUniformMatrix4fv(location, 1, GL_FALSE, value.constData());
        }
    }

    [[nodiscard]] bool isValid() const noexcept { return m_program != 0; }

private:
    void swap(GlShader& other) noexcept {
        std::swap(m_gl, other.m_gl);
        std::swap(m_program, other.m_program);
    }

    QOpenGLFunctions_4_1_Core* m_gl{nullptr};
    GLuint m_program{0};
};

} // namespace cad
