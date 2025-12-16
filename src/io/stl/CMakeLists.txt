# =============================================================================
# IO STL Module
# =============================================================================

add_library(cad_io_stl STATIC
    StlLoader.cpp
)

target_include_directories(cad_io_stl PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cad_io_stl PUBLIC
    cad_io_interface
    cad_core
    Qt6::Core
)
