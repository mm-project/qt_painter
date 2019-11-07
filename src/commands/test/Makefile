QT_INC_DIR="/usr/include/x86_64-linux-gnu/qt5/"
INC_INFO=-I ../ -I $(QT_INC_DIR) -I $(QT_INC_DIR)/QtGui -I $(QT_INC_DIR)/QtCore -I $(QT_INC_DIR)/QtWidgets 
LD_INF=-lQt5Core -lQt5Gui
FLAGS=-DNO_RQ -fPIC -DOS_LINUX -g -fPIC -Wunused-parameter -pipe -std=gnu++17 -frtti -Wall -Wextra -fexceptions -pthread -Wno-unused-function -Wno-unused-variable
UT_BIN_DIR=../../../unit_test_bin

default:
	mkdir -p $(UT_BIN_DIR)
	g++ ${FLAGS} test_shape_creation_directive_commands.cpp ${INC_INFO} ${LD_INF} -o $(UT_BIN_DIR)/test_shape_creation_directive_commands
	g++ ${FLAGS} test_load_save_commands.cpp ${INC_INFO} ${LD_INF} -o $(UT_BIN_DIR)/test_load_save_commands
	g++ ${FLAGS} test_delete_command.cpp ${INC_INFO} ${LD_INF} -o $(UT_BIN_DIR)/test_delete_command
