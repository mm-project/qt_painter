#!/bin/bash
exit 0
for f in `cat tests.lst`; do
    echo "***" > $f/Readme.md
    echo "Test: $f" >> $f/Readme.md
    echo "Creator: Levon" >> $f/Readme.md
    echo "Maintainer: Levon" >> $f/Readme.md
    echo "***" >> $f/Readme.md
    echo >> $f/Readme.md
    echo >> $f/Readme.md
    echo "Summary:" >> $f/Readme.md
    echo "Testing steps: " >> $f/Readme.md
    echo "Expected results: " >> $f/Readme.md
    
    kate $f/Readme.md
done
