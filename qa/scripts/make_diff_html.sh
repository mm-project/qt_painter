#!/bin/bash
f=$1
g=$2

cat html_diff.template | sed "s/%fname1%/$g/" | sed "s/%fname2%/$f/" > $f.html
sed -i "s/%f1%/cat $g/e" $f.html
sed -i "s/%f2%/cat $f/e" $f.html
