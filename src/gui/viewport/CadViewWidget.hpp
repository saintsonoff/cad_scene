/// @file    CadViewWidget.hpp
/// @brief   OpenGL viewport widget.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include "Application.hpp"
#include "Camera.hpp"
#include "SceneRenderer.hpp"
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLWidget>

namespace cad {

/// @brief OpenGL viewport widget with orbit camera and object picking.
class CadViewWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core {
    Q_OBJECT

public:
    explicit CadViewWidget(Application& app, QWidget* parent = nullptr);
    ~CadViewWidget() override;

    [[nodiscard]] Camera& camera() { return m_camera; }
    [[nodiscard]] const Camera& camera() const { return m_camera; }

    void frameScene();
    void frameSelection();
    void frameObject(ObjectId id);
    void resetView();

signals:
    void objectClicked(ObjectId id);
    void contextMenuRequested(const QPoint& globalPos);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void handlePicking(int x, int y);

    Application& m_app;
    Camera m_camera;
    SceneRenderer m_renderer;

    QPoint m_lastMousePos;
    bool m_orbiting{false};
    bool m_panning{false};
};

} // namespace cad
