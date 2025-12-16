# =============================================================================
# GUI Viewport Module
# =============================================================================

add_library(cad_gui_viewport STATIC
    CadViewWidget.cpp
)

target_include_directories(cad_gui_viewport PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cad_gui_viewport PUBLIC
    cad_gui_application
    cad_renderer
    cad_scene
    cad_core
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
)
