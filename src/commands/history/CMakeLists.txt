# Undo/redo stack manager
add_library(cad_commands_history STATIC CommandHistory.cpp)
target_include_directories(cad_commands_history PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cad_commands_history PUBLIC cad_commands_interface cad_core Qt6::Core)
