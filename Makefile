CC:=g++
#FIXME
MOC:=ext/linux_qt/mingw53_32/bin/moc.exe
RULES_DIR:=./
OBJ_ROOT:=./objroot
UNITTEST_BIN_DIR:=$(OBJ_ROOT)/bin
CODE_BASE_DIR:=./src
$(shell mkdir -p $(UNITTEST_BIN_DIR))

QTINCLUDES:=-I ext/linux_qt/mingw53_32/include -I ext/linux_qt/mingw53_32/include/QtWidgets -I ext/linux_qt/mingw53_32/include/QtGui -I ext/linux_qt/mingw53_32/include/QtANGLE -I ext/linux_qt/mingw53_32/include/QtCore

MOCFLAGS:=-DUNICODE -DQT_QML_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_NEEDS_QMAIN -D__GNUC__ -DWIN32 -IC:/Qt_linux/5.7/mingw53_32/mkspecs/win32-g++ -IC:/Users/levons/Desktop/333/PAINTER -IC:/Users/levons/Desktop/333/PAINTER -IC:/Qt_linux/5.7/mingw53_32/include -Iext/linux_qt/mingw53_32/include/QtWidgets -Iext/linux_qt/mingw53_32/include/QtGui -Iext/linux_qt/mingw53_32/include/QtANGLE -Iext/linux_qt/mingw53_32/include/QtCore -I. -IC:/Qt_linux/Tools/mingw530_32/lib/gcc/i686-w64-mingw32/5.3.0/include -IC:/Qt_linux/Tools/mingw530_32/lib/gcc/i686-w64-mingw32/5.3.0/include-fixed -IC:/Qt_linux/Tools/mingw530_32/i686-w64-mingw32/include -IC:/Qt_linux/Tools/mingw530_32/i686-w64-mingw32/include/c++ -IC:/Qt_linux/Tools/mingw530_32/i686-w64-mingw32/include/c++/i686-w64-mingw32 -IC:/Qt_linux/Tools/mingw530_32/i686-w64-mingw32/include/c++/backward

CFLAGS:=-g -Wunused-parameter -std=gnu++0x -Idebug -pipe -fno-keep-inline-dllexport -g -std=gnu++11 -frtti -Wall -Wextra -fexceptions -mthreads -DUNICODE -DQT_QML_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_NEEDS_QMAIN -I..\PAINTER -I. -I..\PAINTER -Iext/linux_qt/mingw53_32/include -I ext/linux_qt/mingw53_32/include/QtWidgets -I ext/linux_qt/mingw53_32/include/QtGui -I ext/linux_qt/mingw53_32/include/QtANGLE -I ext/linux_qt/mingw53_32/include/QtCore -IC:\Qt_linux\5.7\mingw53_32\mkspecs\win32-g++

LDFLAGS:=-static-libstdc++ -Wl,-subsystem,windows -mthreads -lmingw32 -LC:\Qt_linux\5.7\mingw53_32\lib ext/linux_qt/mingw53_32/lib/libqtmaind.a -lshell32 -LC:\utils\my_sql\my_sql\lib -LC:\utils\postgresql\pgsql\lib ext/linux_qt/mingw53_32/lib/libQt5Widgets.a ext/linux_qt/mingw53_32/lib/libQt5Gui.a ext/linux_qt/mingw53_32/lib/libQt5Core.a 

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


$(shell mkdir -p $(TEST_OBJ_ROOT))


$(TEST_NAME):  $(TEST_TARGET) 
	cp ext/windows_qt/runtime_libs/* $(UNITTEST_BIN_DIR)
  
$(TEST_TARGET): $(MOC_CPPS) $(TEST_OBJS) 
	$(CC) $(TEST_OBJS) $(QTINCLUDES) $(LDFLAGS) -o $@ 

$(TEST_OBJ_ROOT)/%.moc.cpp: $(TEST_CODE_DIR)/%.hpp
	$(MOC) $< -o $@
	
	
$(TEST_OBJ_ROOT)/%.o: $(TEST_CODE_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $<  -o $@ 
		
		

