# =============================================================================
# Scene Module (Scene graph)
# =============================================================================

add_library(cad_scene_scene STATIC
    Scene.cpp
)

target_include_directories(cad_scene_scene PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cad_scene_scene PUBLIC
    cad_scene_object
    cad_core
    Qt6::Core
)
