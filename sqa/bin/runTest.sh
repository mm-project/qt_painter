#!/usr/bin/env bash

test=$1
if [ "$test" == "" ]; then
    echo "error: please provide path to test"
    exit 1
fi

cd $test
./run.sh 
