cmake_minimum_required (VERSION 3.0)
include (../../../../etc/bsys/cmake_glavni.cm)

include_directories(../)
include_directories(../../)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../../../unit_test_bin)

make_ut(TESTEE RegionQueryService DEPS ../RegionQueryService.cpp ../RegionQueryService_Dummy.cpp ../rq_object.cpp ../../qt_shapes/line.cpp  ../../qt_shapes/rectangle.cpp  ../../qt_shapes/ellipse.cpp  ../../qt_shapes/polygon.cpp ../../service.cpp )
