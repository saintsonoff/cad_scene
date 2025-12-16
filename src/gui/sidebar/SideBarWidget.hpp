/// @file    SideBarWidget.hpp
/// @brief   Dynamic sidebar with camera and object controls.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include "Application.hpp"
#include "Camera.hpp"
#include "Types.hpp"
#include <QCheckBox>
#include <QColorDialog>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>

namespace cad {

/// @brief Sidebar widget with camera and selected object property controls.
/// @note Context-aware: object controls enabled only when object is selected.
class SideBarWidget : public QWidget {
    Q_OBJECT

public:
    explicit SideBarWidget(Application& app, QWidget* parent = nullptr);

    void setCamera(Camera* camera);

public slots:
    void onSelectionChanged(ObjectId newSelection);
    void refreshFromCamera();

signals:
    void cameraChanged();
    void requestUpdate();

private slots:
    void onCameraDistanceChanged(double value);
    void onCameraYawChanged(double value);
    void onCameraPitchChanged(double value);
    void onCameraFovChanged(double value);
    void onResetCameraClicked();

    void onPositionXChanged(double value);
    void onPositionYChanged(double value);
    void onPositionZChanged(double value);
    void onRotationXChanged(double value);
    void onRotationYChanged(double value);
    void onRotationZChanged(double value);
    void onScaleChanged(double value);
    void onVisibilityChanged(Qt::CheckState state);
    void onShininessChanged(int value);
    void onColorButtonClicked();

private:
    void setupUi();
    void createCameraGroup();
    void createObjectGroup();
    void updateObjectControls();
    void setObjectControlsEnabled(bool enabled);
    void blockObjectSignals(bool block);

    Application& m_app;
    Camera* m_camera{nullptr};
    ObjectId m_selectedId{InvalidObjectId};
    bool m_updatingFromObject{false};

    QGroupBox* m_cameraGroup{nullptr};
    QDoubleSpinBox* m_distanceSpinBox{nullptr};
    QDoubleSpinBox* m_yawSpinBox{nullptr};
    QDoubleSpinBox* m_pitchSpinBox{nullptr};
    QDoubleSpinBox* m_fovSpinBox{nullptr};
    QPushButton* m_resetCameraButton{nullptr};

    QGroupBox* m_objectGroup{nullptr};
    QLabel* m_objectNameLabel{nullptr};
    QDoubleSpinBox* m_posXSpinBox{nullptr};
    QDoubleSpinBox* m_posYSpinBox{nullptr};
    QDoubleSpinBox* m_posZSpinBox{nullptr};
    QDoubleSpinBox* m_rotXSpinBox{nullptr};
    QDoubleSpinBox* m_rotYSpinBox{nullptr};
    QDoubleSpinBox* m_rotZSpinBox{nullptr};
    QDoubleSpinBox* m_scaleSpinBox{nullptr};
    QCheckBox* m_visibleCheckBox{nullptr};
    QSlider* m_shininessSlider{nullptr};
    QLabel* m_shininessValueLabel{nullptr};
    QPushButton* m_colorButton{nullptr};
    QWidget* m_colorPreview{nullptr};
};

} // namespace cad
