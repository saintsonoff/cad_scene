add_subdirectory(interface)
add_subdirectory(transform)
add_subdirectory(material)
add_subdirectory(history)

# Commands module aggregate
add_library(cad_commands INTERFACE)
target_link_libraries(cad_commands INTERFACE
    cad_commands_interface
    cad_commands_transform
    cad_commands_material
    cad_commands_history
)
