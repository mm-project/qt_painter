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
painterexe="$PAINTER_QA_DIR/../bin/mac/painter"

if [ ! -f "$painterexe" ]; then
    echo "Error: no painter executbale"
    exit 1
fi


tname="$1"
needmotion="$2"
export PAINTER_LOG_MOTION="1"
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
mkdir -p expected
mkdir -p tmp
cp $PAINTER_QA_DIR/etc/samples/run.sh ./ 
cp $PAINTER_QA_DIR/designs/* ./tmp 
chmod 755 run.sh

cd tmp
export PAINTER_LOGS_DIR="../input/"
export PAINTER_LOGFILE_PREFIX="replay"
export PAINTER_LOG_MOTION="1"
$painterexe &> /dev/null
#echo "dicmdQaToolExit" >> ../input/replay.log

#sed -i '/^dicmdguiClickButton -object 0x106:Load$/d' ../input/replay.log
#sed -i '/^incmdDesignLoad[[:space:]]*$/d' ../input/replay.log
#sed -i -E 's|^#o --> Out: dicmdDesignLoad -filename .*/([^/]+\.lvi)$|dicmdDesignLoad -filename \1|' ../input/replay.log

sed -i '' '/^dicmdguiClickButton -object 0x106:Load[[:space:]]*$/d' ../input/replay.log
sed -i '' '/^incmdDesignLoad[[:space:]]*$/d' ../input/replay.log
sed -i '' -E 's|^#o --> Out: dicmdDesignLoad -filename[[:space:]]+.*/([^/]+\.lvi)[[:space:]]*$|dicmdDesignLoad -filename \1|' \../input/replay.log

cd ../
rm -rf tmp

export PAINTER_LOGS_DIR=""
export PAINTER_LOGFILE_PREFIX=""
./run.sh regolden
./run.sh

exit 0
if [ "$?" == 0 ]; then
    cd ../
    rm -rf $tname/output
    cp -r $tname $PAINTER_QA_DIR/tests/
    echo "tests/$tname" >> $PAINTER_QA_DIR/tests.lst
    git add $PAINTER_QA_DIR/tests.lst
    git add $PAINTER_QA_DIR/tests/$tname/* 
fi

