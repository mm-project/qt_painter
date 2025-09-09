#!/usr/bin/env bash
set -e

tmp_dir="/tmp"
dev_painter_root="$tmp_dir/qt_painter"
current_painter_root="/Users/levon.sargsyan/int/qt_painter"

function make_dev_binary
{
    mkdir -p $tmp_dir
    cd $tmp_dir
    if [ ! -d $dev_painter_root ]; then
        git clone https://github.com/mm-project/qt_painter.git
    fi
    cd $dev_painter_root
    git checkout dev
    git pull origin dev
    cmake . -DCMAKE_POLICY_VERSION_MINIMUM=3.5
    make -j8
    cd -
}

function run_test
{
    where=$1
    test_path="$2"
    mode="$3"
    options="$4"

    if [ "$where" == "current" ]; then
        painter_root=$current_painter_root
    elif [ "$where" == "dev" ]; then
        painter_root=$dev_painter_root
    else
        echo "Error"
        exit 1
    fi

    export PAINTER_QA_DIR=$painter_root/sqa
    echo "Running [$where]:  $PAINTER_QA_DIR/sqa/bin/runTest.sh $painter_root/$test_path "$mode" "$options" "
    #$PAINTER_QA_DIR/sqa/bin/runTest.sh $painter_root/$test_path "$mode" "$options" &> /dev/null &
}

function main
{
    test_path="$1"
    mode="$2"
    options="$3"

    #echo "Executing >$test_path< >$mode< >$options<"
    #exit 1
    
    #echo -ne "Building dev .... "
    #make_dev_binary &> /dev/null
    #echo " done"

    run_test "current" $test_path "$mode" "$options"
    run_test "dev" $test_path "$mode" "$options"
}

main "$@"