# =============================================================================
# Scene Module - Aggregator CMakeLists.txt
# =============================================================================

add_subdirectory(geometry)
add_subdirectory(primitives)
add_subdirectory(object)
add_subdirectory(scene)

# Aggregate interface library
add_library(cad_scene INTERFACE)
target_link_libraries(cad_scene INTERFACE
    cad_scene_geometry
    cad_scene_primitives
    cad_scene_object
    cad_scene_scene
)
