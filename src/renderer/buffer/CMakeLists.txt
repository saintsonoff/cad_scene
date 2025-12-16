# VAO/VBO/IBO buffer wrapper
add_library(cad_renderer_buffer INTERFACE)
target_include_directories(cad_renderer_buffer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cad_renderer_buffer INTERFACE cad_core Qt6::OpenGL)
