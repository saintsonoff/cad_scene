# =============================================================================
# IO Interface Module
# =============================================================================

add_library(cad_io_interface INTERFACE)

target_include_directories(cad_io_interface INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cad_io_interface INTERFACE
    cad_core
)
