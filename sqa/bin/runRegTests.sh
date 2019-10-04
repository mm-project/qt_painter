#!/bin/bash

SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"

options="$@"

#echo $DIR
export PAINTER_SQA_ROOT=$DIR/..
res=0
total=0
failed=0
passed=0
crashed=0
for i in `cat $PAINTER_SQA_ROOT/tests.lst`; do
    total=`expr $total + 1`
    cd $PAINTER_SQA_ROOT/$i
        echo -ne  "Running $PAINTER_SQA_ROOT/$i --- "
        a=`./run.sh "$options" &> test.info `
        r=$?
        if [ "$r" == 0 ]; then
            echo -e "\e[32mPass\e[0m"
        elif [ "$r" == 3 ]; then
            echo -e "\e[4;5;41mC R A S H\e[0;25m"
            echo "******stack******"
            cat output/logs/painter.log | grep Layer: | sed "s/ \+/ /g" | cut -d ' ' -f7-99 | grep -v "/" | sed "s/^/      /g"
            echo "==========="
            res=1
            crashed=`expr $crashed + 1`
        else
            echo -e "\e[31mError (code:$r) \e[0m"
            echo "************"
            awk '{ print " ----> ", $0 }' test.info
            echo "==========="
            failed=`expr $failed + 1`
            res=1
        fi
        echo "------------------------------------------------------------------------------------"
    cd - &> /dev/null
done


echo 
echo "Summary:"
echo "         Total:   $total "

if [ "$res" == 0 ]; then
    echo "         ALL TESTS PASS"
    exit 0
else
    echo "         Failed:  $failed"
    echo "         Crashed: $crashed"
    exit 1
fi




