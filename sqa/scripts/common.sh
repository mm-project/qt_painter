#!/bin/bash

exit_code=""
need_dbg=""

function process_options
{
    verbose "process_options"
    unset $ELEN_PAINTER_REGOLDEN
}

function verbose
{
    local msg="$1"
    if [ "$need_dbg" != "" ]; then
        echo $msg
    fi
}

function prepocess
{
    verbose "prepocess..."
    rm -rf output
    mkdir output
    cd output
    cp ../golden/* ./ -r
}

function postprocess
{
    verbose "postprocess..."
    if [ "$exit_code" != "0" ]; then
        echo "Test-failed: Abnormal exit ( code $exit_code )"
        exit 0
    else
        #FIXME super inefficent
        cat painter.out | grep "#\/t" 
        info=`cat painter.out | grep "#\/t" | awk '{print $3}'`
        errs=`echo $info | grep ERROR`
        fails=`echo $info | grep FAIL`
        mismatchs=`echo $info | grep MISMATCH`
        if [ "$errs" == "" ] && [ "$fails" == "" ] && [ "$mismatchs" == "" ]; then 
            echo "TesPass !"
            exit 0
        else
            echo "TestFailed :/"
            exit 0
        fi
    fi
    
}

function run 
{
    local options="$1"
    verbose "running..."
    platform=linux
    toolexe=painter
    toolpath=$PAINTER_SQA_ROOT/../bin/$platform
    tool=$toolpath/$toolexe
    verbose "invocation: $tool "$options" &> painter.out "

    $tool $options &> painter.out
    exit_code=$?
}

function run_test
{
    local args="$1"
    
    prepocess
    run "$args"
    postprocess
}
