/// @file    Application.cpp
/// @brief   Implementation of application dependency container.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#include "Application.hpp"
#include "MeshGeometry.hpp"
#include <QFileInfo>

namespace cad {

Application::Application(QObject* parent)
    : QObject(parent)
    , m_scene(std::make_unique<Scene>(this))
    , m_commandHistory(std::make_unique<CommandHistory>(100, this))
    , m_fileLoaderRegistry(std::make_unique<FileLoaderRegistry>()) {

    connect(m_scene.get(), &Scene::objectAdded, this, [this](ObjectId) {
        emit sceneModified();
    });
    connect(m_scene.get(), &Scene::objectRemoved, this, [this](ObjectId) {
        emit sceneModified();
    });
    connect(m_scene.get(), &Scene::sceneCleared, this, [this]() {
        emit sceneModified();
    });
}

Result<ObjectId> Application::loadFile(const QString& filePath) {
    auto result = m_fileLoaderRegistry->load(filePath);
    if (!result) {
        emit errorOccurred(result.error().toString());
        return std::unexpected(result.error());
    }

    auto geometry = std::make_shared<MeshGeometry>(std::move(result.value()));
    QFileInfo fileInfo(filePath);
    auto object = std::make_shared<SceneObject>(geometry, fileInfo.baseName());

    ObjectId id = m_scene->addObject(object);

    emit fileLoaded(filePath, id);

    return id;
}

void Application::clearScene() {
    m_scene->clear();
    m_commandHistory->clear();
}

void Application::undo() {
    auto result = m_commandHistory->undo();
    if (!result) {
        emit errorOccurred(result.error().message);
    } else {
        emit sceneModified();
    }
}

void Application::redo() {
    auto result = m_commandHistory->redo();
    if (!result) {
        emit errorOccurred(result.error().message);
    } else {
        emit sceneModified();
    }
}

} // namespace cad
