#!/bin/sh
#golden_branch_str="$1"
golden_branch="dev"
debug_branch_str="$1"
debug_branch="$2"
testname_str="$3"
testname="$4"
need_rebuild_str="$5"
need_rebuild="yes"

export PAINTER_QA_XVFB=1

echo "Building and running on >$golden_branch<"
if [ "$need_rebuild" != "" ]; then
	git checkout $golden_branch
	cmake .
	make -j8
	mv bin/linux/painter bin/linux/painter.golden
fi
cd $testname
export PAINTER_EXE_NAME="painter.golden"
./run
cd -

echo "Building and running on >$debug_branch<"
if [ "$need_rebuild" != "" ]; then
	git checkout $debug_branch
	cmake .
	make -j8
fi
export PAINTER_EXE_NAME="painter"
export PAINTER_TEST_OUT_DIRNAME="output_debug"
cd $testname
./run
cd - 
