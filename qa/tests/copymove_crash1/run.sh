#!/bin/bash
if [ -z "$PAINTER_QA_DIR" ]; then
    echo "please set \$PAINTER_QA_DIR first"
    exit -1
fi

source $PAINTER_QA_DIR/scripts/test_setup.sh
process_options "$@"
run_test "-replay ../input/crash.log"

