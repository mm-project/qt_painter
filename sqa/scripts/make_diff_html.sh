#!/usr/bin/env bash
f=$1
g=$2

cat html_diff.template | sed "s/%fname1%/$g/" | sed "s/%fname2%/$f/" > $f.html

### if MAC
if [[ "$(uname)" == "Darwin" ]]; then
    gsed -i 's|%f1%|cat "'"$g"'"|e' "$f.html"
    gsed -i 's|%f2%|cat "'"$f"'"|e' "$f.html"
else
    sed -i "s/%f1%/cat $g/e" $f.html
    sed -i "s/%f2%/cat $f/e" $f.html
fi
