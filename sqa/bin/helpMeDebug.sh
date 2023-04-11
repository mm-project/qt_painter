#!/bin/sh

set -e 
#golden_branch_str="$1"
golden_branch="dev"
debug_branch_str="$1"
debug_branch="$2"
testname_str="$3"
testname="$4"
need_rebuild_str="$5"
need_rebuild="$6"

export PAINTER_QA_XVFB=1
echo 
echo "------------------------------------------------"

echo "Building and running on >$golden_branch<"
#if [ "$need_rebuild" != "" ]; then
	#git checkout $golden_branch
	#cmake . 
	#make -j8 
	#cp bin/linux/painter bin/linux/painter.golden
	cp bin/linux/painter.golden bin/linux/painter
#fi
cd $testname
export PAINTER_EXE_NAME="painter"
export PAINTER_TEST_OUT_DIRNAME="output"
./run.sh
cd -
rm bin/linux/painter
#exit 0

echo "---------------------------------------------"
echo

echo "Building and running on >$debug_branch<"
if [ "$need_rebuild" != "" ]; then
	#echo ""
	git checkout $debug_branch
	cmake .
	make -j8 
	cp bin/linux/painter bin/linux/painter.tmp
fi
export PAINTER_EXE_NAME="painter.tmp"
export PAINTER_TEST_OUT_DIRNAME="output_debug"
cd $testname
./run.sh
cd - 



