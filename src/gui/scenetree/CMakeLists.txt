# =============================================================================
# GUI Scene Tree Module
# =============================================================================

add_library(cad_gui_scenetree STATIC
    SceneTreeWidget.cpp
)

target_include_directories(cad_gui_scenetree PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cad_gui_scenetree PUBLIC
    cad_gui_application
    cad_commands
    cad_scene
    cad_core
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)
