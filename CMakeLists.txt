cmake_minimum_required(VERSION 3.25.2)

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
    GIT_TAG v3.0.0
    )

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
    FetchContent_Populate(ftxui)
    add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# ------------------------------------------------------------------------------
project(ds
    VERSION 1.0 
    DESCRIPTION "deadsearch (ds) is a inimalistic and user-friendly menu utility."
    LANGUAGES C) 

find_package(X11 REQUIRED)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_subdirectory(src)

target_link_libraries(ds "-lX11")
target_link_libraries(ds "-lXinerama")

install(TARGETS ds DESTINATION bin)
