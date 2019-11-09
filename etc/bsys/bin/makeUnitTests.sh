#!/bin/bash

SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a 
symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative 
symlink, we need to resolve it relative to the path where the symlink file was 
located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"

options="$@"

#echo $DIR
export PAINTER_ROOT=$DIR/../../../
export PAINTER_BSYS_ROOT=$DIR/..

export QT_INC_DIR="/usr/include/i386-linux-gnu/qt5/"
export QT_LIB_DIR="/usr/lib/i386-linux-gnu"

if [ ! -d $QT_INC_DIR ]; then
    export QT_INC_DIR="/usr/include/x86_64-linux-gnu/qt5/"
    export QT_LIB_DIR="/usr/lib/x86_64-linux-gnu/"
fi

for entry in `cat $PAINTER_BSYS_ROOT/projects.lst`; do
    #echo $entry
    proj=$PAINTER_ROOT/$entry
    if [ -d $proj/test ] && [ -f $proj/test/Makefile ]; then
        cd $proj/test 
        make
    fi
done
