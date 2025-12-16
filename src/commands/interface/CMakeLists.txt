# =============================================================================
# Commands Interface Module
# =============================================================================

add_library(cad_commands_interface INTERFACE)

target_include_directories(cad_commands_interface INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cad_commands_interface INTERFACE
    cad_core
)
