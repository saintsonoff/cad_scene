# GPU mesh buffer cache
add_library(cad_renderer_cache INTERFACE)
target_include_directories(cad_renderer_cache INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cad_renderer_cache INTERFACE cad_renderer_buffer cad_scene cad_core Qt6::OpenGL)
