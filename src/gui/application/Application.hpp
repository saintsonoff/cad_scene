/// @file    Application.hpp
/// @brief   Dependency Injection Container (NO Singletons).
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include "CommandHistory.hpp"
#include "FileLoaderRegistry.hpp"
#include "Scene.hpp"
#include <QObject>
#include <memory>

namespace cad {

/// @brief Application dependency container.
/// @note All services are explicitly constructed and passed via references.
class Application : public QObject {
    Q_OBJECT

public:
    explicit Application(QObject* parent = nullptr);
    ~Application() override = default;

    [[nodiscard]] Scene& scene() { return *m_scene; }
    [[nodiscard]] const Scene& scene() const { return *m_scene; }

    [[nodiscard]] CommandHistory& commandHistory() { return *m_commandHistory; }
    [[nodiscard]] const CommandHistory& commandHistory() const { return *m_commandHistory; }

    [[nodiscard]] FileLoaderRegistry& fileLoaderRegistry() { return *m_fileLoaderRegistry; }
    [[nodiscard]] const FileLoaderRegistry& fileLoaderRegistry() const { return *m_fileLoaderRegistry; }

    /// @brief Load geometry file and add to scene.
    Result<ObjectId> loadFile(const QString& filePath);

    /// @brief Clear all objects and command history.
    void clearScene();

    void undo();
    void redo();

signals:
    void sceneModified();
    void fileLoaded(const QString& path, ObjectId id);
    void errorOccurred(const QString& message);

private:
    std::unique_ptr<Scene> m_scene;
    std::unique_ptr<CommandHistory> m_commandHistory;
    std::unique_ptr<FileLoaderRegistry> m_fileLoaderRegistry;
};

} // namespace cad
