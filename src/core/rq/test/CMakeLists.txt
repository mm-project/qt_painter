cmake_minimum_required (VERSION 3.0)
include (../../../../etc/bsys/cmake_glavni.cm)

include_directories(../)
include_directories(../../)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../../../unit_test_bin)

#make_ut(TESTEE RegionQueryService DEPS ../RegionQueryService.cpp ../rq_object.cpp ../../shapes.cpp ../../service.cpp )
