#!/bin/sh

if [ ! -d 3rd/cpptoolbox ]; then
    mkdir -p 3rd
    cd 3rd/
    git clone https://github.com/lukaszgemborowski/cpptoolbox.git
    cd cpptoolbox/
    git checkout 5bb45da72cc15cd3b6c1e46166dfa3f6e8115bb5
    cd ../..
fi