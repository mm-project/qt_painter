cmake_minimum_required (VERSION 3.0)
project (painter)

add_subdirectory (${PROJECT_SOURCE_DIR}/src/io)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/core/rq)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/core)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/commands)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/gui)

add_executable(painter ${PROJECT_SOURCE_DIR}/src/main.cpp )
add_dependencies(painter rq io core commands gui )
target_link_libraries (painter gui rq io core commands rq Qt5::Widgets Qt5::Core Qt5::Gui ${LD_AUX})
