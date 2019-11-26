#!/bin/bash

SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"

#echo $DIR
export PAINTER_SQA_ROOT=$DIR/..
res=0
for i in `cat $PAINTER_SQA_ROOT/tests.lst`; do
    cd $PAINTER_SQA_ROOT/$i
        echo -ne  "Running $PAINTER_SQA_ROOT/$i --- "
        a=`./run.sh &> test.info `
        r=$?
        if [ "$r" == 0 ]; then
            echo "pass"
        elif [ "$r" == 3 ]; then
            echo "CRASH"
            res=1
        else
            echo "error"
            echo "************"
            cat test.info
            echo "==========="
            res=1
        fi
    cd - &> /dev/null
done


if [ "$res" == 0 ]; then
    echo "ALL TESTS PASS"
    exit 0
else
    exit 1
fi




