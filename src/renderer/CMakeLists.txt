# =============================================================================
# Renderer Module - Aggregator CMakeLists.txt
# =============================================================================

add_subdirectory(camera)
add_subdirectory(buffer)
add_subdirectory(shader)
add_subdirectory(cache)
add_subdirectory(renderer)

# Aggregate interface library
add_library(cad_renderer INTERFACE)
target_link_libraries(cad_renderer INTERFACE
    cad_renderer_camera
    cad_renderer_buffer
    cad_renderer_shader
    cad_renderer_cache
    cad_renderer_renderer
)
