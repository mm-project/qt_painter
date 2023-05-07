cmake_minimum_required (VERSION 3.0)
project (painter)
include (etc/bsys/cmake_glavni.cm)

add_subdirectory (${PROJECT_SOURCE_DIR}/src/io)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/core/qt_shapes)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/core/rq)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/core)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/commands)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/gui)

add_executable(painter ${PROJECT_SOURCE_DIR}/src/main.cpp )
add_dependencies(painter qt_shapes rq io core commands gui )
target_link_libraries (painter gui qt_shapes rq io core commands rq TBB::tbb Qt5::Widgets Qt5::Core Qt5::Gui ${LD_AUX})
