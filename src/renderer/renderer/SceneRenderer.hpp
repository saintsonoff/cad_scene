/// @file    SceneRenderer.hpp
/// @brief   Main OpenGL scene renderer.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include "Camera.hpp"
#include "Error.hpp"
#include "GlShader.hpp"
#include "MeshCache.hpp"
#include "Scene.hpp"
#include <QOpenGLFunctions_4_1_Core>

namespace cad {

/// @brief OpenGL scene renderer with PBR shading.
/// @note Call cleanup() while OpenGL context is current before destruction.
class SceneRenderer {
public:
    SceneRenderer() = default;
    ~SceneRenderer() = default;

    SceneRenderer(const SceneRenderer&) = delete;
    SceneRenderer& operator=(const SceneRenderer&) = delete;

    /// @brief Initialize renderer with OpenGL context.
    Result<void> initialize(QOpenGLFunctions_4_1_Core* gl);

    /// @brief Release all OpenGL resources.
    void cleanup();

    /// @brief Render scene from camera viewpoint.
    void render(const Scene& scene, const Camera& camera);

    /// @brief Update viewport size.
    void resize(int width, int height);

    void setBackgroundColor(const Color& color) { m_backgroundColor = color; }

private:
    void renderObject(const SceneObject& obj, const Camera& camera);
    void renderGrid(const Camera& camera);

    QOpenGLFunctions_4_1_Core* m_gl{nullptr};
    GlShader m_pbrShader;
    GlShader m_gridShader;
    MeshCache m_meshCache;
    GlBuffer m_gridBuffer;

    Color m_backgroundColor{45, 50, 55};

    GLint m_locModel{-1};
    GLint m_locViewProjection{-1};
    GLint m_locNormalMatrix{-1};
    GLint m_locCameraPos{-1};
    GLint m_locBaseColor{-1};
    GLint m_locMetallic{-1};
    GLint m_locRoughness{-1};
    GLint m_locShininess{-1};
    GLint m_locSelected{-1};
};

} // namespace cad
