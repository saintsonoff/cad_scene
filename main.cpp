/// @file    main.cpp
/// @brief   Application entry point for CAD Scene Viewer.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#include "MainWindow.hpp"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("CAD Scene Viewer");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Veidemlmarov Technologies");

    // Configure default OpenGL format
    // OpenGL 4.1 Core - compatible with both macOS and Windows
    QSurfaceFormat format;
    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setSamples(4);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    QSurfaceFormat::setDefaultFormat(format);

    // Create and show main window (owns all dependencies via Application)
    cad::MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
