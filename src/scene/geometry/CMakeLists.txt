# =============================================================================
# Scene Geometry Module
# =============================================================================

add_library(cad_scene_geometry INTERFACE)

target_include_directories(cad_scene_geometry INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cad_scene_geometry INTERFACE
    cad_core
)
