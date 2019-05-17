cmake_minimum_required (VERSION 3.0)
project (painter)

set(CMAKE_AUTOMOC ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/.tmp/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/.tmp/lib)
if(WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/windows/)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/linux/)
endif(WIN32)

find_package (Qt5Widgets)
find_package (Qt5Core)
find_package (Qt5Gui)

set ( CMAKE_CXX_FLAGS "-g -fPIC -Wunused-parameter -pipe -std=gnu++11 -frtti -Wall -Wextra -fexceptions -pthread -Wno-unused-function -Wno-unused-variable")

include_directories(/usr/include/i386-linux-gnu/qt5)
include_directories(/usr/include/i386-linux-gnu/qt5/QtWidgets)
include_directories(/usr/include/i386-linux-gnu/qt5/QtCore)
include_directories(/usr/include/i386-linux-gnu/qt5/QtGui)


add_subdirectory (${PROJECT_SOURCE_DIR}/src/io)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/core)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/commands)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/gui)

add_executable(painter ${PROJECT_SOURCE_DIR}/src/main.cpp )
add_dependencies(painter io core commands gui)
target_link_libraries (painter io core commands gui Qt5::Widgets Qt5::Core Qt5::Gui )



