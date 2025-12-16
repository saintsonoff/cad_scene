# =============================================================================
# Core Types Module
# =============================================================================

add_library(cad_core_types INTERFACE)

target_include_directories(cad_core_types INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cad_core_types INTERFACE
    Qt6::Core
    Qt6::Gui
)
