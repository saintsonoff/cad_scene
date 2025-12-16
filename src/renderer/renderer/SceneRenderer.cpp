/// @file    SceneRenderer.cpp
/// @brief   Implementation of OpenGL scene renderer.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#include "SceneRenderer.hpp"
#include "Shaders.hpp"

namespace cad {

Result<void> SceneRenderer::initialize(QOpenGLFunctions_4_1_Core* gl) {
    m_gl = gl;
    m_meshCache.setGlFunctions(gl);

    auto pbrResult = m_pbrShader.compile(gl, shaders::PbrVertexShader, shaders::PbrFragmentShader);
    if (!pbrResult) {
        return std::unexpected(pbrResult.error());
    }

    m_pbrShader.bind();
    m_locModel = m_pbrShader.uniformLocation("uModel");
    m_locViewProjection = m_pbrShader.uniformLocation("uViewProjection");
    m_locNormalMatrix = m_pbrShader.uniformLocation("uNormalMatrix");
    m_locCameraPos = m_pbrShader.uniformLocation("uCameraPos");
    m_locBaseColor = m_pbrShader.uniformLocation("uBaseColor");
    m_locMetallic = m_pbrShader.uniformLocation("uMetallic");
    m_locRoughness = m_pbrShader.uniformLocation("uRoughness");
    m_locShininess = m_pbrShader.uniformLocation("uShininess");
    m_locSelected = m_pbrShader.uniformLocation("uSelected");
    m_pbrShader.unbind();

    auto gridResult = m_gridShader.compile(gl, shaders::GridVertexShader, shaders::GridFragmentShader);
    if (!gridResult) {
        return std::unexpected(gridResult.error());
    }

    GeometryData gridData;
    const float gridSize = 50.0f;
    const int gridLines = 100;
    const float step = gridSize * 2.0f / static_cast<float>(gridLines);

    for (int i = 0; i <= gridLines; ++i) {
        float pos = -gridSize + static_cast<float>(i) * step;

        gridData.vertices.push_back({{-gridSize, 0.0f, pos}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}});
        gridData.vertices.push_back({{ gridSize, 0.0f, pos}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}});

        gridData.vertices.push_back({{pos, 0.0f, -gridSize}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}});
        gridData.vertices.push_back({{pos, 0.0f,  gridSize}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}});
    }

    for (std::uint32_t i = 0; i < gridData.vertices.size(); ++i) {
        gridData.indices.push_back(i);
    }

    m_gridBuffer.create(gl, gridData);

    gl->glEnable(GL_DEPTH_TEST);
    gl->glEnable(GL_CULL_FACE);
    gl->glCullFace(GL_BACK);
    gl->glEnable(GL_BLEND);
    gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return {};
}

void SceneRenderer::cleanup() {
    m_meshCache.cleanup();
    m_gridBuffer.cleanup();
    m_pbrShader.cleanup();
    m_gridShader.cleanup();
}

void SceneRenderer::render(const Scene& scene, const Camera& camera) {
    if (!m_gl) return;

    // Clear
    m_gl->glClearColor(
        m_backgroundColor.redF(),
        m_backgroundColor.greenF(),
        m_backgroundColor.blueF(),
        1.0f
    );
    m_gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render grid
    renderGrid(camera);

    // Render visible objects
    for (const auto& obj : scene.visibleObjects()) {
        renderObject(*obj, camera);
    }
}

void SceneRenderer::renderObject(const SceneObject& obj, const Camera& camera) {
    auto* buffer = m_meshCache.getOrCreate(obj.id(), obj.geometry());
    if (!buffer || !buffer->isValid()) {
        return;
    }

    m_pbrShader.bind();

    // Matrices
    Mat4 model = obj.modelMatrix();
    Mat4 viewProj = camera.viewProjectionMatrix();

    // Normal matrix (inverse transpose of upper-left 3x3 of model matrix)
    QMatrix3x3 normalMat = model.normalMatrix();

    m_pbrShader.setUniform(m_locModel, model);
    m_pbrShader.setUniform(m_locViewProjection, viewProj);

    if (m_locNormalMatrix >= 0) {
        m_gl->glUniformMatrix3fv(m_locNormalMatrix, 1, GL_FALSE, normalMat.constData());
    }

    m_pbrShader.setUniform(m_locCameraPos, camera.position());

    // Material
    const Material& mat = obj.material();
    m_pbrShader.setUniform(m_locBaseColor, mat.baseColorVec3());
    m_pbrShader.setUniform(m_locMetallic, mat.metallic);
    m_pbrShader.setUniform(m_locRoughness, mat.roughness);
    m_pbrShader.setUniform(m_locShininess, mat.shininess);
    m_pbrShader.setUniform(m_locSelected, obj.isSelected() ? 1.0f : 0.0f);

    buffer->draw();

    m_pbrShader.unbind();
}

void SceneRenderer::renderGrid(const Camera& camera) {
    if (!m_gridBuffer.isValid()) {
        return;
    }

    m_gridShader.bind();

    GLint locVP = m_gridShader.uniformLocation("uViewProjection");
    m_gridShader.setUniform(locVP, camera.viewProjectionMatrix());

    m_gl->glDisable(GL_CULL_FACE);
    m_gl->glDepthMask(GL_FALSE);

    m_gridBuffer.bind();
    m_gl->glDrawElements(GL_LINES, m_gridBuffer.indexCount(), GL_UNSIGNED_INT, nullptr);
    m_gridBuffer.unbind();

    m_gl->glDepthMask(GL_TRUE);
    m_gl->glEnable(GL_CULL_FACE);

    m_gridShader.unbind();
}

void SceneRenderer::resize(int width, int height) {
    if (m_gl) {
        m_gl->glViewport(0, 0, width, height);
    }
}

} // namespace cad
