add_subdirectory(types)
add_subdirectory(concepts)
add_subdirectory(constants)
add_subdirectory(error)

# Core module aggregate
add_library(cad_core INTERFACE)
target_link_libraries(cad_core INTERFACE
    cad_core_types
    cad_core_concepts
    cad_core_constants
    cad_core_error
)
