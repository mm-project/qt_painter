#!/bin/bash

SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"

PAINTER_QA_DIR=$DIR/..

tst_lst=$1
if [ "$tst_lst" == "" ]; then
    tst_lst=$PAINTER_QA_DIR/tests.lst
fi

file_len=`wc -l $tst_lst | cut -d' ' -f1`
b=0
e=1
declare -A PIDS 

function runAll
{
    threads_num=8
    testnum_in_chunk=`expr $file_len / $threads_num`
    e=`expr $b + $testnum_in_chunk`
    echo "Running tests in parralel"
    echo "Threads: $threads_num ( tests running in 1 thread: $testnum_in_chunk )"
    echo
    #echo "file_len: $file_len"
    while [[ "$e" -le "$file_len" ]]; do
        i=`expr $i + 1`
        runParallel $i $b $e
        diff=`expr $file_len - $e`
        b=`expr $b + $testnum_in_chunk`
        e=`expr $e + $testnum_in_chunk`
    done

    #diff=`expr $file_len - $e`
    if [[ "$diff" -gt 0 ]]; then
        runParallel 999 $b 999
    fi

    echo
}

function runParallel
{
    i=$1
    beg=$2
    end=$3
    beg=`expr $beg + 1`
    
    outfile="chunk$i.OUT"
    export PAINTER_QA_TEST_RUN_PARALLEL=1
    $DIR/runRegTests.sh $beg $end &> $outfile &
    pid=$!
    PIDS[$pid]=$outfile
    echo "($pid) -> running: $DIR/runRegTests.sh $beg $end &> ${PIDS[$pid]}  &"
    #sleep 1
}

function waitForAll
{
    all_finished=false
    while [ "$all_finished" != "true" ]; do
        #echo "waiting.."
         all_finished="true"
         for current_pid in "${!PIDS[@]}"; do
             r=`ps -o pid= -p $current_pid`
             if [ "$r" != "" ]; then
                #echo " ---> not finished ${PIDS[$current_pid]}" 
                all_finished="false"
                break
             else
                printResult ${PIDS[$current_pid]}
                PIDS[$current_pid]=""
             fi
         done
     done
    #echo "All finshed."
 }
 
IFS="
"
passs=0

function printResult
{
    fname=$1
    if [ "$fname" != "" ]; then
    #for current_pid in "${!PIDS[@]}"; do
    #   fname=${PIDS[$current_pid]}
        cat $fname
        #echo "finshed chunk: $fname"
        for line in `cat $fname`; do
            need=`echo $line | grep Running`
            if [ "$need" != "" ]; then
                ispass=`echo $line | cut -d' ' -f4 | grep ass`
                #echo "---$line"
                if [ "$ispass" != "" ]; then
                    passs=`expr $passs + 1`
                fi
            fi
            #echo $line
        done
    #done
        #echo "==="
        #echo
    fi
}

function reportAll
{
    fails=0

    echo 
    echo "Summary:"
    echo "         Total:   $file_len "
    echo

    fails=`expr $file_len - $passs`
    if [ "$fails" == 0 ]; then
        echo "         ALL TESTS PASS"
        exit 0
    else
        echo "         Failed:  $fails"
        echo "         Passed:  $passs"
        exit 1
    fi
}


runAll
waitForAll
reportAll
