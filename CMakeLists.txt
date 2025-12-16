# =============================================================================
# CAD Scene Viewer - Root CMakeLists.txt
# =============================================================================
# Modern C++23 Qt/OpenGL CAD Application
# Architecture: Extreme modular tree structure (one class per directory)
# Cross-Platform: macOS + Windows (uses CMakePresets.json)
# =============================================================================

cmake_minimum_required(VERSION 3.25)

project(veidemlmarov_technologies_cad_scene 
    VERSION 1.0.0
    DESCRIPTION "Industrial-grade 3D CAD Scene Viewer"
    LANGUAGES CXX
)

# =============================================================================
# Global C++ Standard Configuration (Cross-Platform)
# =============================================================================

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =============================================================================
# Qt Configuration
# =============================================================================

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS 
    Core
    Gui
    Widgets 
    OpenGL
    OpenGLWidgets
)

# =============================================================================
# Extreme Modular Subdirectory Structure
# =============================================================================

add_subdirectory(src)

# =============================================================================
# Main Executable
# =============================================================================

qt_add_executable(${PROJECT_NAME}
    MANUAL_FINALIZATION
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        cad_gui
)

# =============================================================================
# Platform Bundle Settings (Cross-Platform)
# =============================================================================

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.veidemlmarov.cadscene"
    MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
    WIN32_EXECUTABLE TRUE
)

# =============================================================================
# Installation
# =============================================================================

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(${PROJECT_NAME})
