#!/usr/bin/env bash

SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"

#echo $DIR
export PAINTER_QA_DIR=$DIR/..

platform=linux
if [[ "$(uname)" == "Darwin" ]]; then
    platform=mac
fi
toolexe=painter
toolpath=$PAINTER_QA_DIR/../bin/$platform
tool=$toolpath/$toolexe

painterexe="$toolpath/$toolexe"

if [ ! -f "$painterexe" ]; then
    echo "Error: no painter executbale"
    exit 1
fi

mode_str="$1"
mode="$2"

tname_str="$3"
tname="$4"

needmotion_str="$5"
needmotion="$6"

design_name_str="$7"
design_name="$8"

if [ "$tname" == "" ]; then
    echo "Eror: Provide test name to operate"
    exit 1
fi


if [ "$mode" == "ready" ]; then
    cd $tname
    ./run.sh
    if [ "$?" == "0" ]; then
        cd ../
        rm -rf $tname/output
        cp -r $tname $PAINTER_QA_DIR/tests/
        echo "tests/$tname" >> $PAINTER_QA_DIR/tests.lst
        git add $PAINTER_QA_DIR/tests.lst
        git add $PAINTER_QA_DIR/tests/$tname/*
    else
        echo "Error: can't add failing test"
        exit 1
    fi
else

    if [ -d "$tname" ]; then
        echo "Errot: test with $t exists in current directory"
        exit 1
    fi

    mkdir -p $tname
    cd $tname
    mkdir -p expected
    mkdir -p tmp
    cp $PAINTER_QA_DIR/etc/samples/run.sh ./ 
    cp $PAINTER_QA_DIR/designs/* ./tmp 
    chmod 755 run.sh

    cd tmp
    export PAINTER_LOGS_DIR="../input/"
    export PAINTER_LOGFILE_PREFIX="replay"
    export ELEN_PAINTER_TESTCREATION="1"
    if [ "$needmotion" == "true" ]; then
        export PAINTER_LOG_MOTION="1"
    fi

    if [ "$design_name" != "" ]; then
        if [ ! -f $design_name ]; then
            echo "Error $design_name not found"
            exit 1
        fi
        #echo "dicmdDesignLoad -filename $design_name" >> init.log
        echo "### adding design from $design_name" >> init.log
        cat $design_name >> init.log
        echo >> init.log
        echo "### added design from $design_name" >> init.log
        echo >> init.log
        $painterexe -replay init.log &> /dev/null
    else
        $painterexe &> /dev/null
    fi


    cd ../
    rm -rf tmp

    export PAINTER_LOGS_DIR=""
    export PAINTER_LOGFILE_PREFIX=""
    unset PAINTER_LOG_MOTION
    unset ELEN_PAINTER_TESTCREATION

    ./run.sh regolden
    ./run.sh
fi


