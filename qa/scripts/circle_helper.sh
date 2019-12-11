#!/bin/bash
export PAINTER_SQA_ROOT=~/project/sqa
git remote rm origin
git remote add origin http://github.com/mm-project/qt_painter
git config --global user.name "levibyte"
git config --global user.email "kartol@gmx.com"
git fetch
