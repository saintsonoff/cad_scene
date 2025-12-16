# =============================================================================
# Renderer Main Module
# =============================================================================

add_library(cad_renderer_renderer STATIC
    SceneRenderer.cpp
)

target_include_directories(cad_renderer_renderer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cad_renderer_renderer PUBLIC
    cad_renderer_cache
    cad_renderer_shader
    cad_renderer_camera
    cad_scene
    cad_core
    Qt6::OpenGL
)
