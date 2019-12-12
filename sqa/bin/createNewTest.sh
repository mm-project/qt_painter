#!/bin/bash

SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"

#echo $DIR
export PAINTER_QA_DIR=$DIR/..
painterexe="$PAINTER_QA_DIR/../bin/linux/painter"

if [ ! -f "$painterexe" ]; then
    echo "Error: no painter executbale"
    exit 1
fi


tname="$1"
needmotion="$2"
export PAINTER_LOG_MOTION="$needmotion"
if [ "$tname" == "" ]; then
    echo "Eror: Provide test name to create"
    exit 1
fi


if [ -d "$tname" ]; then
    echo "Errot: test with $t exists in current directory"
    exit 1
fi

mkdir -p $tname
cd $tname
mkdir -p golden
mkdir -p tmp
cp $PAINTER_QA_DIR/etc/samples/run.sh ./ 
chmod 755 run.sh

cd tmp
export PAINTER_LOGS_DIR="../input/"
export PAINTER_LOGFILE_PREFIX="replay"
export PAINTER_LOG_MOTION=1
$painterexe
echo "dicmdQaToolExit" >> ../input/replay.log
cd ../
rm -rf tmp

export PAINTER_LOGS_DIR=""
export PAINTER_LOGFILE_PREFIX=""
./run.sh regolden
./run.sh

cd ../

if [ "$?" == 0 ]; then
    rm -rf $tname/output
    cp $tname $PAINTER_QA_DIR/tests/ -r
    echo "tests/$tname" >> $PAINTER_QA_DIR/tests.lst
    git add $PAINTER_QA_DIR/tests.lst
    git add $PAINTER_QA_DIR/tests/$tname/* 
fi

