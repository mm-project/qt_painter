#!/bin/bash
if [ -z "$PAINTER_SQA_ROOT" ]; then
    echo "please set \$PAINTER_SQA_ROOT first"
    exit -1
fi

source $PAINTER_SQA_ROOT/scripts/common.sh
process_options "$@"
run_test "-replay ../input/replay.log"

