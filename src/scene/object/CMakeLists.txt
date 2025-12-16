# =============================================================================
# Scene Object Module
# =============================================================================

add_library(cad_scene_object STATIC
    SceneObject.cpp
)

target_include_directories(cad_scene_object PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cad_scene_object PUBLIC
    cad_scene_geometry
    cad_core
)
