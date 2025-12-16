# Main application window
add_library(cad_gui_mainwindow STATIC MainWindow.cpp)
target_include_directories(cad_gui_mainwindow PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cad_gui_mainwindow PUBLIC
    cad_gui_application cad_gui_viewport cad_gui_sidebar cad_gui_scenetree
    cad_scene cad_core Qt6::Core Qt6::Gui Qt6::Widgets
)
