# =============================================================================
# GUI Application Module
# =============================================================================

add_library(cad_gui_application STATIC
    Application.cpp
)

target_include_directories(cad_gui_application PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cad_gui_application PUBLIC
    cad_scene
    cad_commands
    cad_io
    cad_core
    Qt6::Core
    Qt6::Widgets
)
