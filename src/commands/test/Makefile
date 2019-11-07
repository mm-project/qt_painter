QT_INC_DIR="/usr/include/x86_64-linux-gnu/qt5/"
INC_INFO=-I ../ -I $(QT_INC_DIR) -I $(QT_INC_DIR)/QtGui -I $(QT_INC_DIR)/QtCore -I $(QT_INC_DIR)/QtWidgets 
LD_INF=-lQt5Core -lQt5Gui
FLAGS=-DNO_RQ -fPIC
UT_BIN_DIR=../../../unit_test_bin

default:
	mkdir -p $(UT_BIN_DIR)
	g++ ${FLAGS} test_shape_creation_directive_commands.cpp ${INC_INFO} ${LD_INF} -o $(UT_BIN_DIR)/test_shape_creation_directive_commands
	g++ ${FLAGS} test_load_save_commands.cpp ${INC_INFO} ${LD_INF} -o $(UT_BIN_DIR)/test_load_save_commands
