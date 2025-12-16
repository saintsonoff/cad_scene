add_subdirectory(interface)
add_subdirectory(stl)
add_subdirectory(registry)

# IO module aggregate
add_library(cad_io INTERFACE)
target_link_libraries(cad_io INTERFACE
    cad_io_interface
    cad_io_stl
    cad_io_registry
)
