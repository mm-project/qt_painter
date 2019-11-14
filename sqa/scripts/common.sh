#!/bin/bash

exit_code=""
need_dbg=""
succ=4

export PAINTER_LOGFILE_PREFIX="painter"
#mode="regolden"
mode=""
extras=""

function extra_comparision {
    extras="$1"
}

function process_options
{
    verbose "process_options"
    unset ELEN_PAINTER_REGOLDEN
    mode=$1
    #if [ "$1" == "regolden" ]; then
    #    mode="regolden"
    #fi
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
        export ELEN_PAINTER_REGOLDEN="1"
    fi
    
    if [ "$mode" = "debug" ]; then
        export ELEN_PAINTER_TESTDBG="1"
    fi
    
    verbose "prepocess..."
    rm -rf output
    mkdir -p golden
    mkdir output
    cd output
    
    cp $PAINTER_SQA_ROOT/etc/webrelated/* . -r
    
    if [ "$mode" != "regolden" ]; then
        cp ../golden/* ./ -rf
        if [ "$?" != 0 ]; then
            succ=`expr $succ - 1`
        fi
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
            for e in $extras; do
                cp -rf $e ../golden/$e.golden
            done
            cp ./logs/painter.log ../golden/painter.log.golden
            cp ./logs/painter.lvi ../golden/painter.lvi.golden
            #cp painter.out ../golden/painter.out.golden
            cp `find -name "*.golden*"` ../golden
            #cnvscprs=`find -name "*compare*"`
            #for i in $cnvscprs; do
            #    cp $i ../golden/$i.golden
            #done
            echo "Succesfully Regoldened"
            
            return 0;
        fi
        
        if [ -d "../golden" ]; then
            a=`diff ./logs/painter.log painter.log.golden`
            b=`diff ./logs/painter.lvi painter.lvi.golden`
            #c=`diff painter.out painter.out.golden`
            c=""

            res_d=""
            problems=""
            for ex in $extras; do
                d=`diff $ex $ex.golden`
                if [ "$d" != "" ]; then
                    problems="$ex <--> $ex.golden $problems"
                    #files[${ex}]="$ex.golden"
                fi
                res_d="$res_d$d"
            done

        else
            succ=`expr $succ - 1`
            echo "CAN'T FIND GOLDEN DIR"
        fi
        #if [ "$a" == "" ] && [ "$b" == "" ]; then
        #    succ=3
        #else
            declare -A files
            if [ "$a" != "" ]; then
                echo "MISMTACH: logs/painter.log <--> painter.log.golden "
                files["logs/painter.log"]="painter.log.golden"
                succ=`expr $succ - 1`
            fi
        
            if [ "$b" != "" ]; then
                succ=`expr $succ - 1`
                echo "MISMTACH: logs/painter.lvi <--> painter.lvi.golden "
                files["logs/painter.lvi"]="painter.lvi.golden"
            fi

            if [ "$c" != "" ]; then
                #succ=`expr $succ - 1`
                echo "MISMTACH: painter.out <--> painter.out.golden "
            fi

            if [ "$res_d" != "" ]; then
                succ=`expr $succ - 1`
                echo "MISMTACH: $problems "
            fi

        #fi
        
        #FIXME super inefficent
        #cat painter.out | grep "#\/t" 
        #info=`cat painter.out | grep "#\/t" | awk '{print $3}'`
        #errs=`echo $info | grep ERROR`
        #fails=`echo $info | grep FAIL`
        mismatchs=`cat logs/painter.log | grep "\#e --> Error: " | grep MISMATCH | cut -d' ' -f5-6  | sed "s/ / <--> /"`
        if [ "$mismatchs" != "" ]; then 
            cat logs/painter.log | grep "\#e --> Error: " | grep MISMATCH | cut -d' ' -f5-6  | sed "s/ / <--> /" | sed "s/^/MISMATCH: /" 
            #echo "OTHER MISMATCH(s) "
            #echo $mismatchs
            #succ=`expr $succ - 1`
            #exit 0
        fi
        
        if [ "$mode" = "compare" ]; then
            for k in ${!files[@]}; do
                kompare ${files[$k]} $k
            done
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

    $tool $options &> painter.output
    exit_code=$?
}

function run_test
{
    local args="$1"
    
    prepocess
    run "$args"
    postprocess
}
