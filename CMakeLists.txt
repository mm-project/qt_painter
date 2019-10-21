cmake_minimum_required (VERSION 3.0)
project (painter)

set(LD_AUX "") 
set(QTDIR "/usr/lib/x86_64-linux-gnu")
#windows
if (WIN32) 
	# on visual
	if (MSVC)
            set ( CMAKE_CXX_FLAGS "-DOS_WINDOWS")
            set(QTDIR "C:\\Programs\\Qt\\5.11.1")
	# on min mingw
	else()
            set ( CMAKE_CXX_FLAGS "-DOS_LINUX -g -fPIC -Wunused-parameter -pipe -std=gnu++17 -frtti -Wall -Wextra -fexceptions -pthread -Wno-unused-function -Wno-unused-variable")
            set(QTDIR "./ext/mingw73_32")
	endif(MSVC)

	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/windows/)
	set(QT_INCLUDE_DIR ${QTDIR}/include)
#linux
else()
	set(LD_AUX "dl") 
	set(CMAKE_CXX_FLAGS "-DOS_LINUX -g -fPIC -Wunused-parameter -pipe -std=gnu++17 -frtti -Wall -Wextra -fexceptions -pthread -Wno-unused-function -Wno-unused-variable")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/linux/)
    #linux64
    if (EXISTS "/usr/include/x86_64-linux-gnu/qt5/")
            set(QT_INCLUDE_DIR "/usr/include/x86_64-linux-gnu/qt5/")
    else()
    #linux32
            set(QT_INCLUDE_DIR "/usr/include/i386-linux-gnu/qt5/")
    endif()
endif (WIN32)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/.tmp/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/.tmp/lib)

set(CMAKE_AUTOMOC ON)
set(CMAKE_PREFIX_PATH ${QT_INCLUDE_DIR})
if (MSVC)
	set(CMAKE_CXX_STANDARD 17)
endif (MSVC)

find_package (Qt5Widgets)
find_package (Qt5Core)
find_package (Qt5Gui)
#find_package (Qt5Svg)
#find_package (Qt5Xml)

#set(QT_INCLUDE_DIR ${QTDIR}/include)
include_directories(${QT_INCLUDE_DIR}/)
include_directories(${QT_INCLUDE_DIR}/QtCore)
include_directories(${QT_INCLUDE_DIR}/QtGui)
include_directories(${QT_INCLUDE_DIR}/QtCore)
include_directories(${QT_INCLUDE_DIR}/QtWidgets)


add_subdirectory (${PROJECT_SOURCE_DIR}/src/io)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/core/rq)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/core)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/commands)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/gui)

add_executable(painter ${PROJECT_SOURCE_DIR}/src/main.cpp )
add_dependencies(painter rq io core commands gui )
target_link_libraries (painter gui rq io core commands rq Qt5::Widgets Qt5::Core Qt5::Gui ${LD_AUX})
