#!/usr/bin/env bash
f_orig=$1
g=$2
d=$f_orig

f=$(echo $f_orig | sed 's/^CanvasFor_//; s/\.png$//')
g=$(echo $g | sed 's/^CanvasFor_//; s/\.png$//')
d=$(echo $d | sed 's/^CanvasFor_//; s/\.png$//')

cat html_diff.template | sed "s/%fname1%/$g/" | sed "s/%fname2%/$f/" | sed "s/%fname3%/$d/" > $f_orig.html

### if MAC
if [[ "$(uname)" == "Darwin" ]]; then
    gsed -i 's|%f1%|cat "'"$g"'"|e' "$f_orig.html"
    gsed -i 's|%f2%|cat "'"$f"'"|e' "$f_orig.html"
    gsed -i 's|%f3%|cat "'"$d"'"|e' "$f_orig.html"
else
    sed -i "s/%f1%/cat $g/e" $f_orig.html
    sed -i "s/%f2%/cat $f/e" $f_orig.html
    sed -i "s/%f2%/cat $d/e" $f_orig.html
fi
