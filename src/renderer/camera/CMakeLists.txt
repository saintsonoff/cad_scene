# =============================================================================
# Renderer Camera Module
# =============================================================================

add_library(cad_renderer_camera STATIC
    Camera.cpp
)

target_include_directories(cad_renderer_camera PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cad_renderer_camera PUBLIC
    cad_core
)
