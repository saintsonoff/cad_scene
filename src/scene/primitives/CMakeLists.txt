# =============================================================================
# Scene Primitives Module
# =============================================================================

add_library(cad_scene_primitives INTERFACE)

target_include_directories(cad_scene_primitives INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cad_scene_primitives INTERFACE
    cad_scene_geometry
    cad_core
)
