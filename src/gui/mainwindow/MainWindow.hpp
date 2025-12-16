/// @file    MainWindow.hpp
/// @brief   Main application window.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include "Application.hpp"
#include "CadViewWidget.hpp"
#include "SceneTreeWidget.hpp"
#include "SideBarWidget.hpp"
#include <QDockWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>

namespace cad {

/// @brief Main application window with viewport and dock panels.
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onFileOpen();
    void onFileClear();
    void onEditUndo();
    void onEditRedo();
    void onViewResetCamera();
    void onViewFrameAll();
    void onViewFrameSelection();
    void onHelpAbout();

    void onFileLoaded(const QString& path, ObjectId id);
    void onErrorOccurred(const QString& message);
    void onHistoryChanged();

    void onFocusObjectRequested(ObjectId id);

private:
    void setupUi();
    void createMenus();
    void createToolBar();
    void createStatusBar();
    void createDockWidgets();
    void connectSignals();
    void updateUndoRedoActions();

    Application m_app;

    CadViewWidget* m_viewport{nullptr};
    SideBarWidget* m_sideBar{nullptr};
    SceneTreeWidget* m_sceneTree{nullptr};
    QDockWidget* m_sideBarDock{nullptr};
    QDockWidget* m_sceneTreeDock{nullptr};

    QAction* m_undoAction{nullptr};
    QAction* m_redoAction{nullptr};
};

} // namespace cad
