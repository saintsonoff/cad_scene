# Property editor sidebar widget
add_library(cad_gui_sidebar STATIC SideBarWidget.cpp)
target_include_directories(cad_gui_sidebar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cad_gui_sidebar PUBLIC
    cad_gui_application cad_commands cad_renderer_camera cad_scene cad_core
    Qt6::Core Qt6::Gui Qt6::Widgets
)
