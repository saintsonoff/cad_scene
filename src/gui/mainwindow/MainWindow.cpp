/// @file    MainWindow.cpp
/// @brief   Implementation of main application window.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#include "MainWindow.hpp"
#include "CubeGeometry.hpp"
#include "MeshGeometry.hpp"
#include "SphereGeometry.hpp"
#include "TorusGeometry.hpp"
#include <QApplication>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

namespace cad {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    setupUi();
    connectSignals();

    auto cubeGeom = std::make_shared<MeshGeometry>(CubeGeometry::generate(1.5f));
    auto cube = std::make_shared<SceneObject>(cubeGeom, "Demo Cube");
    cube->setPosition({-2.0f, 0.75f, 0.0f});
    cube->setBaseColor(QColor(100, 149, 237));
    m_app.scene().addObject(cube);

    auto sphereGeom = std::make_shared<MeshGeometry>(SphereGeometry::generate(1.0f));
    auto sphere = std::make_shared<SceneObject>(sphereGeom, "Demo Sphere");
    sphere->setPosition({2.0f, 1.0f, 0.0f});
    sphere->setBaseColor(QColor(255, 99, 71));
    m_app.scene().addObject(sphere);

    auto torusGeom = std::make_shared<MeshGeometry>(TorusGeometry::generate(1.2f, 0.4f));
    auto torus = std::make_shared<SceneObject>(torusGeom, "Demo Torus");
    torus->setPosition({0.0f, 1.0f, 2.5f});
    torus->setBaseColor(QColor(50, 205, 50));
    m_app.scene().addObject(torus);

    m_viewport->frameScene();

    m_sideBar->setCamera(&m_viewport->camera());
    m_sideBar->refreshFromCamera();

    setWindowTitle(tr("CAD Scene Viewer"));
    resize(1400, 900);
}

void MainWindow::setupUi() {
    // Central viewport
    m_viewport = new CadViewWidget(m_app, this);
    setCentralWidget(m_viewport);

    createMenus();
    createToolBar();
    createStatusBar();
    createDockWidgets();
}

void MainWindow::createMenus() {
    // File menu
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));

    QAction* openAction = fileMenu->addAction(tr("&Open..."), this, &MainWindow::onFileOpen);
    openAction->setShortcut(QKeySequence::Open);

    fileMenu->addSeparator();

    fileMenu->addAction(tr("&Clear Scene"), this, &MainWindow::onFileClear);

    fileMenu->addSeparator();

    QAction* exitAction = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
    exitAction->setShortcut(QKeySequence::Quit);

    // Edit menu
    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));

    m_undoAction = editMenu->addAction(tr("&Undo"), this, &MainWindow::onEditUndo);
    m_undoAction->setShortcut(QKeySequence::Undo);
    m_undoAction->setEnabled(false);

    m_redoAction = editMenu->addAction(tr("&Redo"), this, &MainWindow::onEditRedo);
    m_redoAction->setShortcut(QKeySequence::Redo);
    m_redoAction->setEnabled(false);

    // View menu
    QMenu* viewMenu = menuBar()->addMenu(tr("&View"));

    viewMenu->addAction(tr("&Reset Camera"), this, &MainWindow::onViewResetCamera);

    QAction* frameAllAction = viewMenu->addAction(tr("Frame &All"), this, &MainWindow::onViewFrameAll);
    frameAllAction->setShortcut(QKeySequence(Qt::Key_Home));

    QAction* frameSelAction = viewMenu->addAction(tr("Frame &Selection"), this, &MainWindow::onViewFrameSelection);
    frameSelAction->setShortcut(QKeySequence(Qt::Key_F));

    viewMenu->addSeparator();

    // Toggle sidebar
    // Will be added after dock is created

    // Help menu
    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&About"), this, &MainWindow::onHelpAbout);
}

void MainWindow::createToolBar() {
    QToolBar* toolBar = addToolBar(tr("Main"));
    toolBar->setObjectName("MainToolBar");

    toolBar->addAction(tr("Open"), this, &MainWindow::onFileOpen);
    toolBar->addSeparator();
    toolBar->addAction(m_undoAction);
    toolBar->addAction(m_redoAction);
    toolBar->addSeparator();
    toolBar->addAction(tr("Frame All"), this, &MainWindow::onViewFrameAll);
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createDockWidgets() {
    // Scene Tree dock (Outliner) - Left side
    m_sceneTreeDock = new QDockWidget(tr("Scene Hierarchy"), this);
    m_sceneTreeDock->setObjectName("SceneTreeDock");
    m_sceneTreeDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    m_sceneTree = new SceneTreeWidget(m_app, this);
    m_sceneTreeDock->setWidget(m_sceneTree);

    addDockWidget(Qt::LeftDockWidgetArea, m_sceneTreeDock);

    // Sidebar dock (Properties) - Right side
    m_sideBarDock = new QDockWidget(tr("Properties"), this);
    m_sideBarDock->setObjectName("PropertiesDock");
    m_sideBarDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    m_sideBar = new SideBarWidget(m_app, this);
    m_sideBarDock->setWidget(m_sideBar);

    addDockWidget(Qt::RightDockWidgetArea, m_sideBarDock);

    // Add toggle actions to view menu
    QMenu* viewMenu = nullptr;
    for (QAction* action : menuBar()->actions()) {
        if (action->text() == tr("&View")) {
            viewMenu = action->menu();
            break;
        }
    }
    if (viewMenu) {
        viewMenu->addAction(m_sceneTreeDock->toggleViewAction());
        viewMenu->addAction(m_sideBarDock->toggleViewAction());
    }
}

void MainWindow::connectSignals() {
    // Application signals
    connect(&m_app, &Application::fileLoaded, this, &MainWindow::onFileLoaded);
    connect(&m_app, &Application::errorOccurred, this, &MainWindow::onErrorOccurred);
    connect(&m_app.commandHistory(), &CommandHistory::historyChanged, this, &MainWindow::onHistoryChanged);

    // Sidebar signals
    connect(m_sideBar, &SideBarWidget::requestUpdate, m_viewport, QOverload<>::of(&QWidget::update));
    connect(m_sideBar, &SideBarWidget::cameraChanged, this, [this]() {
        m_sideBar->refreshFromCamera();
    });

    // Scene tree signals
    connect(m_sceneTree, &SceneTreeWidget::requestUpdate, m_viewport, QOverload<>::of(&QWidget::update));
    connect(m_sceneTree, &SceneTreeWidget::focusObjectRequested, this, &MainWindow::onFocusObjectRequested);

    // Viewport signals
    connect(m_viewport, &CadViewWidget::objectClicked, this, [this](ObjectId id) {
        Q_UNUSED(id);
        m_sideBar->refreshFromCamera();
    });
}

void MainWindow::updateUndoRedoActions() {
    m_undoAction->setEnabled(m_app.commandHistory().canUndo());
    m_redoAction->setEnabled(m_app.commandHistory().canRedo());

    if (m_app.commandHistory().canUndo()) {
        m_undoAction->setText(tr("&Undo %1").arg(m_app.commandHistory().undoDescription()));
    } else {
        m_undoAction->setText(tr("&Undo"));
    }

    if (m_app.commandHistory().canRedo()) {
        m_redoAction->setText(tr("&Redo %1").arg(m_app.commandHistory().redoDescription()));
    } else {
        m_redoAction->setText(tr("&Redo"));
    }
}

// Slots
void MainWindow::onFileOpen() {
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString filter = m_app.fileLoaderRegistry().fileFilter();

    QString filePath = QFileDialog::getOpenFileName(this, tr("Open 3D Model"),
                                                    documentsPath, filter);
    if (!filePath.isEmpty()) {
        m_app.loadFile(filePath);
    }
}

void MainWindow::onFileClear() {
    m_app.clearScene();
    m_viewport->resetView();
    statusBar()->showMessage(tr("Scene cleared"));
}

void MainWindow::onEditUndo() {
    m_app.undo();
    m_viewport->update();
    m_sideBar->onSelectionChanged(m_app.scene().selectedObjectId());
}

void MainWindow::onEditRedo() {
    m_app.redo();
    m_viewport->update();
    m_sideBar->onSelectionChanged(m_app.scene().selectedObjectId());
}

void MainWindow::onViewResetCamera() {
    m_viewport->resetView();
    m_sideBar->refreshFromCamera();
}

void MainWindow::onViewFrameAll() {
    m_viewport->frameScene();
    m_sideBar->refreshFromCamera();
}

void MainWindow::onViewFrameSelection() {
    m_viewport->frameSelection();
    m_sideBar->refreshFromCamera();
}

void MainWindow::onHelpAbout() {
    QMessageBox::about(this, tr("About CAD Scene Viewer"),
        tr("<h3>CAD Scene Viewer</h3>"
           "<p>Version 1.0.0</p>"
           "<p>A modern 3D CAD viewer built with:</p>"
           "<ul>"
           "<li>C++23</li>"
           "<li>Qt 6</li>"
           "<li>OpenGL 4.1 Core</li>"
           "</ul>"
           "<p>Cross-platform: macOS / Windows</p>"));
}

void MainWindow::onFileLoaded(const QString& path, ObjectId id) {
    Q_UNUSED(id);
    statusBar()->showMessage(tr("Loaded: %1").arg(path), 5000);
    m_viewport->frameScene();
    m_sideBar->refreshFromCamera();
}

void MainWindow::onErrorOccurred(const QString& message) {
    statusBar()->showMessage(tr("Error: %1").arg(message), 5000);
    QMessageBox::warning(this, tr("Error"), message);
}

void MainWindow::onHistoryChanged() {
    updateUndoRedoActions();
}

void MainWindow::onFocusObjectRequested(ObjectId id) {
    // Focus camera on the requested object
    auto obj = m_app.scene().findObject(id);
    if (obj) {
        m_viewport->frameObject(id);
        m_sideBar->refreshFromCamera();
        statusBar()->showMessage(tr("Focused on: %1").arg(obj->name()), 2000);
    }
}

void MainWindow::closeEvent(QCloseEvent* event) {
    event->accept();
}

} // namespace cad
