#!/bin/bash

for f in `cat tests.lst`; do
    echo "***" > $f/Readme.md
    echo "Test: $f" >> $f/Readme.md
    echo "Creator: Levon" >> $f/Readme.md
    echo "Maintainer: Levon" >> $f/Readme.md
    echo "***" >> $f/Readme.md
    echo >> $f/Readme.md
    echo "Brief description: " >> $f/Readme.md
    echo "Testing steps: " >> $f/Readme.md
    echo "Expected results: " >> $f/Readme.md
    
    #kate $f/Readme.md
done
