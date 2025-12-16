# GLSL shader program wrapper
add_library(cad_renderer_shader INTERFACE)
target_include_directories(cad_renderer_shader INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cad_renderer_shader INTERFACE cad_core Qt6::OpenGL)
