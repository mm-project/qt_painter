cmake_minimum_required (VERSION 3.0)
include (../../../etc/bsys/cmake_glavni.cm)


include_directories(../)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../../unit_test_bin)

make_ut(TESTEE shape_creation_directive_commands DEPS "" )
make_ut(TESTEE load_save_commands DEPS "" )
make_ut(TESTEE delete_command DEPS "" )

