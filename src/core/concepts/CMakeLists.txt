# C++20 concepts for type constraints
add_library(cad_core_concepts INTERFACE)
target_include_directories(cad_core_concepts INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cad_core_concepts INTERFACE cad_core_types)
