#!/bin/bash

exit_code=""
need_dbg=""

export PAINTER_LOGFILE_PREFIX="painter"
#mode="regolden"

function process_options
{
    verbose "process_options"
    unset $ELEN_PAINTER_REGOLDEN
    if [ "$1" == "regolden" ]; then
        mode="regolden"
    fi
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
    if [ "$mode" = "regolden" ]; then
        export ELEN_PAINTER_REGOLDEN=1
    fi
    
    verbose "prepocess..."
    rm -rf output
    mkdir output
    cd output
    if [ "$mode" != "regolden" ]; then
        cp ../golden/* ./ -rf
    fi
}

function postprocess
{
    
    verbose "postprocess..."
    if [ "$exit_code" != "0" ]; then
        echo "Test-failed: Abnormal exit ( code $exit_code )"
        exit 3
    else
        if [ "$mode" = "regolden" ]; then
            echo "Regoldning..."
            rm -rf ../golden 
            mkdir -p ../golden
            cp ./logs/painter.log ../golden/painter.log.golden
            cp ./logs/painter.lvi ../golden/painter.lvi.golden
            cp painter.out ../golden/painter.out.golden
            cnvscprs=`find -name "*canvas_compare*"`
            for i in $cnvscprs; do
                cp $i ../golden/$i.golden
            done
            echo "Succesfully Regoldened"
            
            return 0;
        fi
        
        succ=4
        if [ -d "../golden" ]; then
            a=`diff ./logs/painter.log painter.log.golden`
            b=`diff ./logs/painter.lvi painter.lvi.golden`
            c=`diff painter.out painter.out.golden`
        else
            succ=`expr $succ - 1`
            echo "CAN'T FIND GOLDEN DIR"
        fi
        #if [ "$a" == "" ] && [ "$b" == "" ]; then
        #    succ=3
        #else
            if [ "$a" != "" ]; then
                echo "MISMTACH ./logs/painter.log <--> painter.log.golden "
                succ=`expr $succ - 1`
            fi
        
            if [ "$b" != "" ]; then
                succ=`expr $succ - 1`
                echo "MISMTACH ./logs/painter.lvi <--> painter.lvi.golden "
            fi

            if [ "$c" != "" ]; then
                succ=`expr $succ - 1`
                echo "MISMTACH painter.out <--> painter.out.golden "
            fi

        #fi
        
        #FIXME super inefficent
        cat painter.out | grep "#\/t" 
        info=`cat painter.out | grep "#\/t" | awk '{print $3}'`
        errs=`echo $info | grep ERROR`
        fails=`echo $info | grep FAIL`
        mismatchs=`echo $info | grep MISMATCH`
        if [ "$errs" != "" ] || [ "$fails" != "" ] || [ "$mismatchs" != "" ]; then 
            #echo "TesPass !"
            #exit 0
        #else
            echo "OTHER MISMATCH :/"
            #succ=`expr $succ - 1`
            #exit 0
        fi
        
        if [ "$succ" != 4 ]; then
            echo "Test failed."
            exit 1
        else
            echo "Test Pass !"
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
