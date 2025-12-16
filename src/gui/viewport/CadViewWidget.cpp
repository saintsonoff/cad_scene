/// @file    CadViewWidget.cpp
/// @brief   Implementation of OpenGL viewport widget.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#include "CadViewWidget.hpp"
#include "Constants.hpp"
#include <QMouseEvent>
#include <QWheelEvent>

namespace cad {

CadViewWidget::CadViewWidget(Application& app, QWidget* parent)
    : QOpenGLWidget(parent)
    , m_app(app) {

    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    connect(&m_app, &Application::sceneModified, this, QOverload<>::of(&QWidget::update));
}

CadViewWidget::~CadViewWidget() {
    makeCurrent();
    m_renderer.cleanup();
    doneCurrent();
}

void CadViewWidget::initializeGL() {
    if (!initializeOpenGLFunctions()) {
        qCritical("Failed to initialize OpenGL functions");
        return;
    }

    auto result = m_renderer.initialize(this);
    if (!result) {
        qCritical("Renderer init failed: %s", qPrintable(result.error().toString()));
    }
}

void CadViewWidget::resizeGL(int w, int h) {
    m_camera.setViewport(w, h);
    m_renderer.resize(w, h);
}

void CadViewWidget::paintGL() {
    m_renderer.render(m_app.scene(), m_camera);
}

void CadViewWidget::mousePressEvent(QMouseEvent* event) {
    m_lastMousePos = event->pos();

    if (event->button() == Qt::LeftButton) {
        if (event->modifiers() & Qt::AltModifier) {
            m_orbiting = true;
        } else {
            // Object picking
            handlePicking(event->pos().x(), event->pos().y());
        }
    } else if (event->button() == Qt::MiddleButton) {
        m_panning = true;
    } else if (event->button() == Qt::RightButton) {
        if (event->modifiers() & Qt::AltModifier) {
            m_orbiting = true;
        } else {
            emit contextMenuRequested(event->globalPosition().toPoint());
        }
    }

    event->accept();
}

void CadViewWidget::mouseMoveEvent(QMouseEvent* event) {
    QPoint delta = event->pos() - m_lastMousePos;
    m_lastMousePos = event->pos();

    if (m_orbiting) {
        m_camera.orbit(static_cast<float>(delta.x()), static_cast<float>(-delta.y()));
        update();
    } else if (m_panning) {
        m_camera.pan(static_cast<float>(delta.x()), static_cast<float>(delta.y()));
        update();
    }

    event->accept();
}

void CadViewWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) {
        m_orbiting = false;
    }
    if (event->button() == Qt::MiddleButton) {
        m_panning = false;
    }

    event->accept();
}

void CadViewWidget::wheelEvent(QWheelEvent* event) {
    float delta = static_cast<float>(event->angleDelta().y()) / constants::WheelDeltaFactor;
    m_camera.zoom(delta);
    update();
    event->accept();
}

void CadViewWidget::handlePicking(int x, int y) {
    Ray ray = m_camera.screenToRay(x, y);
    auto hit = m_app.scene().raycast(ray);

    if (hit) {
        m_app.scene().selectObject(*hit);
        emit objectClicked(*hit);
    } else {
        m_app.scene().clearSelection();
        emit objectClicked(InvalidObjectId);
    }

    update();
}

void CadViewWidget::frameScene() {
    AABB bbox = m_app.scene().boundingBox();
    if (bbox.isValid()) {
        m_camera.frameBox(bbox);
        update();
    }
}

void CadViewWidget::frameSelection() {
    if (auto obj = m_app.scene().selectedObject()) {
        m_camera.frameBox(obj->boundingBox());
        update();
    }
}

void CadViewWidget::frameObject(ObjectId id) {
    if (auto obj = m_app.scene().findObject(id)) {
        m_camera.frameBox(obj->boundingBox());
        update();
    }
}

void CadViewWidget::resetView() {
    m_camera.reset();
    update();
}

} // namespace cad
