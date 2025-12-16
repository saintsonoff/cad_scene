add_subdirectory(application)
add_subdirectory(viewport)
add_subdirectory(sidebar)
add_subdirectory(scenetree)
add_subdirectory(mainwindow)

# GUI module aggregate
add_library(cad_gui INTERFACE)
target_link_libraries(cad_gui INTERFACE
    cad_gui_application
    cad_gui_viewport
    cad_gui_sidebar
    cad_gui_scenetree
    cad_gui_mainwindow
)
