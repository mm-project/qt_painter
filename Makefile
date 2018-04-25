CC:=g++
#FIXME
MOC:=ext/linux_qt/mingw53_32/bin/moc.exe
RULES_DIR:=./
OBJ_ROOT:=./.tmp/objroot
UNITTEST_BIN_DIR:=./bin
CODE_BASE_DIR:=./src


MINGW_QT_ROOT=./ext/linux_qt/mingw53_32
QT_ROOT=$(MINGW_QT_ROOT)

QT_INCL_ROOT=$(QT_ROOT)/include
QT_LIB_ROOT=$(QT_ROOT)/lib
QTINCLUDES:=-I $(QT_INCL_ROOT) -I $(QT_INCL_ROOT)/QtWidgets -I $(QT_INCL_ROOT)/QtGui -I $(QT_INCL_ROOT)/QtCore

#MOCFLAGS:=-DUNICODE -DQT_QML_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_NEEDS_QMAIN -D__GNUC__ -DWIN32 -IC:/Qt_linux/5.7/mingw53_32/mkspecs/win32-g++ -IC:/Users/levons/Desktop/333/PAINTER -IC:/Users/levons/Desktop/333/PAINTER -IC:/Qt_linux/5.7/mingw53_32/include -I$(QT_INCL_ROOT)/QtWidgets -I$(QT_INCL_ROOT)/QtGui -I$(QT_INCL_ROOT)/QtANGLE -I$(QT_INCL_ROOT)/QtCore -I. -IC:/Qt_linux/Tools/mingw530_32/lib/gcc/i686-w64-mingw32/5.3.0/include -IC:/Qt_linux/Tools/mingw530_32/lib/gcc/i686-w64-mingw32/5.3.0/include-fixed -IC:/Qt_linux/Tools/mingw530_32/i686-w64-mingw32/include -IC:/Qt_linux/Tools/mingw530_32/i686-w64-mingw32/include/c++ -IC:/Qt_linux/Tools/mingw530_32/i686-w64-mingw32/include/c++/i686-w64-mingw32 -IC:/Qt_linux/Tools/mingw530_32/i686-w64-mingw32/include/c++/backward

MOCFLAGS:=-DUNICODE -DQT_QML_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_NEEDS_QMAIN -D__GNUC__ -DWIN32 $(QTINCLUDES)

CFLAGS:=-g -Wunused-parameter -std=gnu++0x -Idebug -pipe -fno-keep-inline-dllexport -g -std=gnu++11 -frtti -Wall -Wextra -fexceptions -mthreads -DUNICODE -DQT_QML_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_NEEDS_QMAIN $(QTINCLUDES)

LDFLAGS:=-static-libstdc++ -Wl,-subsystem,windows -mthreads -lmingw32 -L -lshell32 $(QT_LIB_ROOT)/libqtmain.a $(QT_LIB_ROOT)/libQt5Widgets.a $(QT_LIB_ROOT)/libQt5Gui.a $(QT_LIB_ROOT)/libQt5Core.a 

CXX:=g++
CXXFLAGS:=$(CFLAGS)

MODULE=painter
TEST_NAME:=$(MODULE)Test
TEST_TARGET:=$(UNITTEST_BIN_DIR)/$(TEST_NAME)
TEST_OBJ_ROOT:=$(OBJ_ROOT)/$(MODULE)

TEST_CODE_DIR:=$(CODE_BASE_DIR)/$(MODULE)
CPPS:=$(wildcard $(TEST_CODE_DIR)/*.cpp)
HEADERS:=$(wildcard $(TEST_CODE_DIR)/*.hpp)
OBJS1:=$(CPPS:.cpp=.o)

MOC_HEADERS:=$(shell grep -l Q_OBJECT $(HEADERS))
MOC_INCLUDE:=$(CODE_BASE_DIR)/$(MODULE) $(QTINCLUDES)
OBJS2:=$(MOC_HEADERS:.hpp=.moc.o)

MOC_CPPS1:=$(MOC_HEADERS:.hpp=.moc.cpp)
MOC_CPPS:=$(patsubst $(TEST_CODE_DIR)/%,$(OBJ_ROOT)/$(MODULE)/%,$(MOC_CPPS1))
MOC_OBJS:=$(patsubst $(TEST_CODE_DIR)/%,$(OBJ_ROOT)/$(MODULE)/%,$(OBJS2))
TEST_OBJS:=$(patsubst $(TEST_CODE_DIR)/%,$(OBJ_ROOT)/$(MODULE)/%,$(OBJS1)) $(MOC_OBJS)
TEST_DEPS:=$(TEST_OBJS:.o=.d)

$(shell mkdir -p $(TEST_OBJ_ROOT))
$(shell mkdir -p $(UNITTEST_BIN_DIR))


$(TEST_NAME):  $(TEST_TARGET) 
	cp ext/windows_qt/runtime_libs/* $(UNITTEST_BIN_DIR)
  
$(TEST_TARGET): $(MOC_CPPS) $(TEST_OBJS) 
	$(CC) $(TEST_OBJS) $(QTINCLUDES) $(LDFLAGS) -o $@ 

$(TEST_OBJ_ROOT)/%.moc.cpp: $(TEST_CODE_DIR)/%.hpp
	$(MOC) $< -o $@
	
	
$(TEST_OBJ_ROOT)/%.o: $(TEST_CODE_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $<  -o $@ 
		

-include $(TEST_DEPS)
		
$(TEST_OBJ_ROOT)/%.d: $(TEST_CODE_DIR)/%.cpp 
	$(CC) $<  $(TEST_LOCAL_CC_FLAG) $(TEST_LOCAL_INCS)  -MM -MT $(@:.d=.o) > $@
