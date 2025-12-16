/// @file    SideBarWidget.cpp
/// @brief   Implementation of sidebar widget.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#include "SideBarWidget.hpp"
#include "Constants.hpp"
#include "MaterialCommand.hpp"
#include "TransformCommands.hpp"
#include <QFormLayout>
#include <QSignalBlocker>

namespace cad {

SideBarWidget::SideBarWidget(Application& app, QWidget* parent)
    : QWidget(parent)
    , m_app(app) {
    setupUi();

    connect(&m_app.scene(), &Scene::selectionChanged, this, &SideBarWidget::onSelectionChanged);

    setObjectControlsEnabled(false);
}

void SideBarWidget::setCamera(Camera* camera) {
    m_camera = camera;
    refreshFromCamera();
}

void SideBarWidget::setupUi() {
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(12);

    createCameraGroup();
    mainLayout->addWidget(m_cameraGroup);

    createObjectGroup();
    mainLayout->addWidget(m_objectGroup);

    mainLayout->addStretch();

    setMinimumWidth(constants::SidebarMinWidth);
    setMaximumWidth(constants::SidebarMaxWidth);
}

void SideBarWidget::createCameraGroup() {
    m_cameraGroup = new QGroupBox(tr("Camera"), this);
    auto* layout = new QFormLayout(m_cameraGroup);

    // Distance
    m_distanceSpinBox = new QDoubleSpinBox(this);
    m_distanceSpinBox->setRange(constants::MinCameraDistance, constants::MaxCameraDistance);
    m_distanceSpinBox->setSingleStep(0.5);
    m_distanceSpinBox->setDecimals(1);
    connect(m_distanceSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &SideBarWidget::onCameraDistanceChanged);
    layout->addRow(tr("Distance:"), m_distanceSpinBox);

    // Yaw
    m_yawSpinBox = new QDoubleSpinBox(this);
    m_yawSpinBox->setRange(-360.0, 360.0);
    m_yawSpinBox->setSingleStep(5.0);
    m_yawSpinBox->setDecimals(1);
    m_yawSpinBox->setSuffix("°");
    connect(m_yawSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &SideBarWidget::onCameraYawChanged);
    layout->addRow(tr("Yaw:"), m_yawSpinBox);

    // Pitch
    m_pitchSpinBox = new QDoubleSpinBox(this);
    m_pitchSpinBox->setRange(constants::MinPitch, constants::MaxPitch);
    m_pitchSpinBox->setSingleStep(5.0);
    m_pitchSpinBox->setDecimals(1);
    m_pitchSpinBox->setSuffix("°");
    connect(m_pitchSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &SideBarWidget::onCameraPitchChanged);
    layout->addRow(tr("Pitch:"), m_pitchSpinBox);

    // FOV
    m_fovSpinBox = new QDoubleSpinBox(this);
    m_fovSpinBox->setRange(10.0, 120.0);
    m_fovSpinBox->setSingleStep(5.0);
    m_fovSpinBox->setDecimals(1);
    m_fovSpinBox->setSuffix("°");
    connect(m_fovSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &SideBarWidget::onCameraFovChanged);
    layout->addRow(tr("FOV:"), m_fovSpinBox);

    // Reset button
    m_resetCameraButton = new QPushButton(tr("Reset View"), this);
    connect(m_resetCameraButton, &QPushButton::clicked, this, &SideBarWidget::onResetCameraClicked);
    layout->addRow(m_resetCameraButton);
}

void SideBarWidget::createObjectGroup() {
    m_objectGroup = new QGroupBox(tr("Selected Object"), this);
    auto* layout = new QFormLayout(m_objectGroup);

    // Object name label
    m_objectNameLabel = new QLabel(tr("(none)"), this);
    m_objectNameLabel->setStyleSheet("font-weight: bold;");
    layout->addRow(m_objectNameLabel);

    // Position X
    m_posXSpinBox = new QDoubleSpinBox(this);
    m_posXSpinBox->setRange(-1000.0, 1000.0);
    m_posXSpinBox->setSingleStep(0.1);
    m_posXSpinBox->setDecimals(2);
    connect(m_posXSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &SideBarWidget::onPositionXChanged);
    layout->addRow(tr("X:"), m_posXSpinBox);

    // Position Y
    m_posYSpinBox = new QDoubleSpinBox(this);
    m_posYSpinBox->setRange(-1000.0, 1000.0);
    m_posYSpinBox->setSingleStep(0.1);
    m_posYSpinBox->setDecimals(2);
    connect(m_posYSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &SideBarWidget::onPositionYChanged);
    layout->addRow(tr("Y:"), m_posYSpinBox);

    // Position Z
    m_posZSpinBox = new QDoubleSpinBox(this);
    m_posZSpinBox->setRange(-1000.0, 1000.0);
    m_posZSpinBox->setSingleStep(0.1);
    m_posZSpinBox->setDecimals(2);
    connect(m_posZSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &SideBarWidget::onPositionZChanged);
    layout->addRow(tr("Z:"), m_posZSpinBox);

    // Rotation X (Euler angles for user-friendliness)
    m_rotXSpinBox = new QDoubleSpinBox(this);
    m_rotXSpinBox->setRange(-180.0, 180.0);
    m_rotXSpinBox->setSingleStep(5.0);
    m_rotXSpinBox->setDecimals(1);
    m_rotXSpinBox->setSuffix("°");
    connect(m_rotXSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &SideBarWidget::onRotationXChanged);
    layout->addRow(tr("Rot X:"), m_rotXSpinBox);

    // Rotation Y
    m_rotYSpinBox = new QDoubleSpinBox(this);
    m_rotYSpinBox->setRange(-180.0, 180.0);
    m_rotYSpinBox->setSingleStep(5.0);
    m_rotYSpinBox->setDecimals(1);
    m_rotYSpinBox->setSuffix("°");
    connect(m_rotYSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &SideBarWidget::onRotationYChanged);
    layout->addRow(tr("Rot Y:"), m_rotYSpinBox);

    // Rotation Z
    m_rotZSpinBox = new QDoubleSpinBox(this);
    m_rotZSpinBox->setRange(-180.0, 180.0);
    m_rotZSpinBox->setSingleStep(5.0);
    m_rotZSpinBox->setDecimals(1);
    m_rotZSpinBox->setSuffix("°");
    connect(m_rotZSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &SideBarWidget::onRotationZChanged);
    layout->addRow(tr("Rot Z:"), m_rotZSpinBox);

    // Uniform scale
    m_scaleSpinBox = new QDoubleSpinBox(this);
    m_scaleSpinBox->setRange(0.01, 100.0);
    m_scaleSpinBox->setSingleStep(0.1);
    m_scaleSpinBox->setDecimals(2);
    m_scaleSpinBox->setValue(1.0);
    connect(m_scaleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &SideBarWidget::onScaleChanged);
    layout->addRow(tr("Scale:"), m_scaleSpinBox);

    // Visibility
    m_visibleCheckBox = new QCheckBox(tr("Visible"), this);
    m_visibleCheckBox->setChecked(true);
    connect(m_visibleCheckBox, &QCheckBox::checkStateChanged, this, &SideBarWidget::onVisibilityChanged);
    layout->addRow(m_visibleCheckBox);

    // Shininess (Specular Intensity)
    auto* shininessLayout = new QHBoxLayout();
    m_shininessSlider = new QSlider(Qt::Horizontal, this);
    m_shininessSlider->setRange(0, 100);
    m_shininessSlider->setValue(50);
    connect(m_shininessSlider, &QSlider::valueChanged, this, &SideBarWidget::onShininessChanged);

    m_shininessValueLabel = new QLabel("0.50", this);
    m_shininessValueLabel->setMinimumWidth(35);

    shininessLayout->addWidget(m_shininessSlider);
    shininessLayout->addWidget(m_shininessValueLabel);
    layout->addRow(tr("Shininess:"), shininessLayout);

    // Color
    auto* colorLayout = new QHBoxLayout();
    m_colorButton = new QPushButton(tr("Change..."), this);
    connect(m_colorButton, &QPushButton::clicked, this, &SideBarWidget::onColorButtonClicked);

    m_colorPreview = new QWidget(this);
    m_colorPreview->setFixedSize(24, 24);
    m_colorPreview->setStyleSheet("background-color: #b4b4be; border: 1px solid #555;");

    colorLayout->addWidget(m_colorPreview);
    colorLayout->addWidget(m_colorButton);
    colorLayout->addStretch();
    layout->addRow(tr("Color:"), colorLayout);
}

void SideBarWidget::onSelectionChanged(ObjectId newSelection) {
    m_selectedId = newSelection;
    updateObjectControls();
}

void SideBarWidget::refreshFromCamera() {
    if (!m_camera) return;

    // RAII signal blockers - exception-safe
    const QSignalBlocker distanceBlocker(m_distanceSpinBox);
    const QSignalBlocker yawBlocker(m_yawSpinBox);
    const QSignalBlocker pitchBlocker(m_pitchSpinBox);
    const QSignalBlocker fovBlocker(m_fovSpinBox);

    m_distanceSpinBox->setValue(static_cast<double>(m_camera->distance()));
    m_yawSpinBox->setValue(static_cast<double>(m_camera->yaw()));
    m_pitchSpinBox->setValue(static_cast<double>(m_camera->pitch()));
    m_fovSpinBox->setValue(static_cast<double>(m_camera->fov()));
}

void SideBarWidget::updateObjectControls() {
    auto obj = m_app.scene().findObject(m_selectedId);

    if (!obj) {
        m_objectNameLabel->setText(tr("(none)"));
        setObjectControlsEnabled(false);
        return;
    }

    setObjectControlsEnabled(true);
    m_updatingFromObject = true;

    m_objectNameLabel->setText(obj->name());

    blockObjectSignals(true);

    Vec3 pos = obj->position();
    m_posXSpinBox->setValue(static_cast<double>(pos.x()));
    m_posYSpinBox->setValue(static_cast<double>(pos.y()));
    m_posZSpinBox->setValue(static_cast<double>(pos.z()));

    // Convert quaternion to Euler angles for display
    Quat rot = obj->rotation();
    QVector3D eulerAngles = rot.toEulerAngles();
    m_rotXSpinBox->setValue(static_cast<double>(eulerAngles.x()));
    m_rotYSpinBox->setValue(static_cast<double>(eulerAngles.y()));
    m_rotZSpinBox->setValue(static_cast<double>(eulerAngles.z()));

    Vec3 scale = obj->scale();
    m_scaleSpinBox->setValue(static_cast<double>(scale.x()));

    m_visibleCheckBox->setChecked(obj->isVisible());

    // Material properties
    float shininess = obj->material().shininess;
    m_shininessSlider->setValue(static_cast<int>(shininess * 100.0f));
    m_shininessValueLabel->setText(QString::number(shininess, 'f', 2));

    Color color = obj->baseColor();
    m_colorPreview->setStyleSheet(
        QString("background-color: %1; border: 1px solid #555;").arg(color.name()));

    blockObjectSignals(false);
    m_updatingFromObject = false;
}

void SideBarWidget::setObjectControlsEnabled(bool enabled) {
    m_posXSpinBox->setEnabled(enabled);
    m_posYSpinBox->setEnabled(enabled);
    m_posZSpinBox->setEnabled(enabled);
    m_rotXSpinBox->setEnabled(enabled);
    m_rotYSpinBox->setEnabled(enabled);
    m_rotZSpinBox->setEnabled(enabled);
    m_scaleSpinBox->setEnabled(enabled);
    m_visibleCheckBox->setEnabled(enabled);
    m_shininessSlider->setEnabled(enabled);
    m_colorButton->setEnabled(enabled);
}

void SideBarWidget::blockObjectSignals(bool block) {
    // Note: For simple cases, individual QSignalBlocker RAII guards are preferred.
    // This method is kept for bulk blocking when needed.
    m_posXSpinBox->blockSignals(block);
    m_posYSpinBox->blockSignals(block);
    m_posZSpinBox->blockSignals(block);
    m_rotXSpinBox->blockSignals(block);
    m_rotYSpinBox->blockSignals(block);
    m_rotZSpinBox->blockSignals(block);
    m_scaleSpinBox->blockSignals(block);
    m_visibleCheckBox->blockSignals(block);
    m_shininessSlider->blockSignals(block);
}

// Camera slots
void SideBarWidget::onCameraDistanceChanged(double value) {
    if (m_camera) {
        m_camera->setDistance(static_cast<float>(value));
        emit cameraChanged();
        emit requestUpdate();
    }
}

void SideBarWidget::onCameraYawChanged(double value) {
    if (m_camera) {
        m_camera->orbit(static_cast<float>(value - m_camera->yaw()) / constants::OrbitSensitivity, 0.0f);
        emit cameraChanged();
        emit requestUpdate();
    }
}

void SideBarWidget::onCameraPitchChanged(double value) {
    if (m_camera) {
        m_camera->orbit(0.0f, static_cast<float>(value - m_camera->pitch()) / constants::OrbitSensitivity);
        emit cameraChanged();
        emit requestUpdate();
    }
}

void SideBarWidget::onCameraFovChanged(double value) {
    if (m_camera) {
        m_camera->setFov(static_cast<float>(value));
        emit cameraChanged();
        emit requestUpdate();
    }
}

void SideBarWidget::onResetCameraClicked() {
    if (m_camera) {
        m_camera->reset();
        refreshFromCamera();
        emit cameraChanged();
        emit requestUpdate();
    }
}

// Object slots with Command Pattern
void SideBarWidget::onPositionXChanged(double value) {
    if (m_updatingFromObject || m_selectedId == InvalidObjectId) return;

    if (auto obj = m_app.scene().findObject(m_selectedId)) {
        Vec3 newPos = obj->position();
        newPos.setX(static_cast<float>(value));

        auto cmd = std::make_unique<MoveCommand>(m_app.scene(), m_selectedId, newPos);
        m_app.commandHistory().execute(std::move(cmd));
        emit requestUpdate();
    }
}

void SideBarWidget::onPositionYChanged(double value) {
    if (m_updatingFromObject || m_selectedId == InvalidObjectId) return;

    if (auto obj = m_app.scene().findObject(m_selectedId)) {
        Vec3 newPos = obj->position();
        newPos.setY(static_cast<float>(value));

        auto cmd = std::make_unique<MoveCommand>(m_app.scene(), m_selectedId, newPos);
        m_app.commandHistory().execute(std::move(cmd));
        emit requestUpdate();
    }
}

void SideBarWidget::onPositionZChanged(double value) {
    if (m_updatingFromObject || m_selectedId == InvalidObjectId) return;

    if (auto obj = m_app.scene().findObject(m_selectedId)) {
        Vec3 newPos = obj->position();
        newPos.setZ(static_cast<float>(value));

        auto cmd = std::make_unique<MoveCommand>(m_app.scene(), m_selectedId, newPos);
        m_app.commandHistory().execute(std::move(cmd));
        emit requestUpdate();
    }
}

void SideBarWidget::onRotationXChanged(double value) {
    if (m_updatingFromObject || m_selectedId == InvalidObjectId) return;

    if (auto obj = m_app.scene().findObject(m_selectedId)) {
        // Get current Euler angles
        Quat currentRot = obj->rotation();
        QVector3D euler = currentRot.toEulerAngles();
        euler.setX(static_cast<float>(value));

        // Convert back to quaternion (avoids gimbal lock internally)
        Quat newRot = Quat::fromEulerAngles(euler);

        auto cmd = std::make_unique<RotateCommand>(m_app.scene(), m_selectedId, newRot);
        m_app.commandHistory().execute(std::move(cmd));
        emit requestUpdate();
    }
}

void SideBarWidget::onRotationYChanged(double value) {
    if (m_updatingFromObject || m_selectedId == InvalidObjectId) return;

    if (auto obj = m_app.scene().findObject(m_selectedId)) {
        Quat currentRot = obj->rotation();
        QVector3D euler = currentRot.toEulerAngles();
        euler.setY(static_cast<float>(value));

        Quat newRot = Quat::fromEulerAngles(euler);

        auto cmd = std::make_unique<RotateCommand>(m_app.scene(), m_selectedId, newRot);
        m_app.commandHistory().execute(std::move(cmd));
        emit requestUpdate();
    }
}

void SideBarWidget::onRotationZChanged(double value) {
    if (m_updatingFromObject || m_selectedId == InvalidObjectId) return;

    if (auto obj = m_app.scene().findObject(m_selectedId)) {
        Quat currentRot = obj->rotation();
        QVector3D euler = currentRot.toEulerAngles();
        euler.setZ(static_cast<float>(value));

        Quat newRot = Quat::fromEulerAngles(euler);

        auto cmd = std::make_unique<RotateCommand>(m_app.scene(), m_selectedId, newRot);
        m_app.commandHistory().execute(std::move(cmd));
        emit requestUpdate();
    }
}

void SideBarWidget::onScaleChanged(double value) {
    if (m_updatingFromObject || m_selectedId == InvalidObjectId) return;

    float s = static_cast<float>(value);
    Vec3 newScale(s, s, s);

    auto cmd = std::make_unique<ScaleCommand>(m_app.scene(), m_selectedId, newScale);
    m_app.commandHistory().execute(std::move(cmd));
    emit requestUpdate();
}

void SideBarWidget::onVisibilityChanged(Qt::CheckState state) {
    if (m_updatingFromObject || m_selectedId == InvalidObjectId) return;

    bool visible = (state == Qt::Checked);

    auto cmd = std::make_unique<SetVisibilityCommand>(m_app.scene(), m_selectedId, visible);
    m_app.commandHistory().execute(std::move(cmd));
    emit requestUpdate();
}

void SideBarWidget::onShininessChanged(int value) {
    if (m_updatingFromObject || m_selectedId == InvalidObjectId) return;

    float shininess = static_cast<float>(value) / 100.0f;
    m_shininessValueLabel->setText(QString::number(shininess, 'f', 2));

    if (auto obj = m_app.scene().findObject(m_selectedId)) {
        Material newMaterial = obj->material();
        newMaterial.shininess = shininess;

        auto cmd = std::make_unique<SetMaterialCommand>(m_app.scene(), m_selectedId, newMaterial);
        m_app.commandHistory().execute(std::move(cmd));
        emit requestUpdate();
    }
}

void SideBarWidget::onColorButtonClicked() {
    if (m_selectedId == InvalidObjectId) return;

    auto obj = m_app.scene().findObject(m_selectedId);
    if (!obj) return;

    Color currentColor = obj->baseColor();
    Color newColor = QColorDialog::getColor(currentColor, this, tr("Select Object Color"));

    if (newColor.isValid() && newColor != currentColor) {
        auto cmd = std::make_unique<SetColorCommand>(m_app.scene(), m_selectedId, newColor);
        m_app.commandHistory().execute(std::move(cmd));

        m_colorPreview->setStyleSheet(
            QString("background-color: %1; border: 1px solid #555;").arg(newColor.name()));

        emit requestUpdate();
    }
}

} // namespace cad
