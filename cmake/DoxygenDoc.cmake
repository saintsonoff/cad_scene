find_package(Doxygen)

option(BUILD_DOCUMENTATION "Build API documentation" ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
        message(WARNING "Doxygen not found. Documentation target disabled.")
        return()
    endif()

    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/Doxyfile")
        message(WARNING "Doxyfile not found. Documentation target disabled.")
        return()
    endif()

    add_custom_target(doc ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()