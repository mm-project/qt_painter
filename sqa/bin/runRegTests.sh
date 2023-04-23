#!/bin/bash

SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"

cdir=$PWD
failed_test_list_file=$PWD/.failed_tsts
rm $failed_test_list_file -f
#exit 0

options="$@"
BEG=$1
END=$2
#echo $DIR
export PAINTER_QA_DIR=$DIR/..
export ARTIFACTS_DIR=$DIR/../../artifacts
export PAINTER_SCRIPTS_DIR=$PAINTER_QA_DIR/scripts
export PYTHON_PATH=$PAINTER_SCRIPTS_DIR:$PYTHON_PATH
export IS_CI=0

mkdir -p $ARTIFACTS_DIR

if [ "$CI_BUILD" == "" ]; then
    CI_BUILD="TCI_UNIT"
fi
if [ "$CI_CHECK" == "2" ];  then
    run_id=`python3 $PAINTER_SCRIPTS_DIR/testrail_binder.py -action create_new_run --projectid 1 --runname $CI_BUILD --suiteid 1 `
    echo "TESTRAIL TestRUN:<$run_id>"
    export IS_CI=2
fi


res=0
total=0
failed=0
passed=0
crashed=0
t_id=1
t_res=5

if [ "$PAINTER_QA_TEST_RUN_PARALLEL" != "" ]; then
    TESTLST=`awk -v b=$BEG -v e=$END 'NR >= b && NR <= e' $PAINTER_QA_DIR/tests.lst`
else
    TESTLST=`cat $PAINTER_QA_DIR/tests.lst`
    #echo "DEBUG: awk -v b=$BEG -v e=$END 'NR >= b && NR <= e' $PAINTER_QA_DIR/tests.lst"
fi

for i in $TESTLST; do
    total=`expr $total + 1`
    cd $PAINTER_QA_DIR/$i
        testname=`basename $PAINTER_QA_DIR/$i` 
        echo -ne  "Running $PAINTER_QA_DIR/$i --- "
        a=`./run.sh "$options" &> test.info `
        r=$?
        if [ "$r" == 0 ]; then
            echo -e "\e[32mPass\e[0m"
            t_res=1
            passed=`expr $passed + 1`
            if [ "$PAINTER_NEED_TEST_PASS_ARTIFACTS" != "" ]; then
	    	cp output $ARTIFACTS_DIR/$testname -rf
	    fi
        elif [ "$r" == 3 ]; then
            echo -e "\e[4;5;41mC R A S H\e[0;25m"
            echo "******stack******"
            cat output/logs/painter.log | grep Layer: | sed "s/ \+/ /g" | cut -d ' ' -f7-99 | grep -v "/" | sed "s/^/      /g"
            echo "==========="
            res=1
            crashed=`expr $crashed + 1`
            cp output $ARTIFACTS_DIR/$testname -rf
	    echo "$i" >> $failed_test_list_file
        else
            echo -e "\e[31mError (code:$r) \e[0m"
            echo "************"
            awk '{ print " ----> ", $0 }' test.info
            echo "==========="
            failed=`expr $failed + 1`
            res=1
            cp output $ARTIFACTS_DIR/$testname -rf
	    echo "$i" >> $failed_test_list_file
        fi
        echo  "------------------------------------------------------------------------------------"
    cd - &> /dev/null
    if [ "$IS_CI" == "1" ];  then
        r=`python3 $PAINTER_SCRIPTS_DIR/testrail_binder.py -action update_test_result --runid $run_id --resultid $t_res --testname "$i" `
        echo "RESULT:<$r>" &> testrail.io
    fi
    t_id=`expr $ti + 1`
done

if [ "$PAINTER_QA_TEST_RUN_PARALLEL" == "" ]; then
    echo 
    echo "Summary:"
    echo "         Total:   $total "
    echo

    if [ "$res" == 0 ]; then
        echo "         ALL TESTS PASS"
        exit 0
    else
        echo "         Failed:  $failed"
        echo "         Passed:  $passed"
        echo "         Crashd:  $crashed"
        echo 
	echo "*****************************************"
	echo
	cat $failed_test_list_file
	echo
	echo "*****************************************"
	exit 1
    fi
else
    if [ "$res" == 0 ]; then
        exit 0
    else
        exit 1
    fi
fi



