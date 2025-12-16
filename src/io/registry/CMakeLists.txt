# =============================================================================
# IO Registry Module
# =============================================================================

add_library(cad_io_registry STATIC
    FileLoaderRegistry.cpp
)

target_include_directories(cad_io_registry PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cad_io_registry PUBLIC
    cad_io_interface
    cad_io_stl
    cad_core
)
