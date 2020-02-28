#!/bin/sh

if [ ! -d 3rd/cpptoolbox ]; then
    mkdir -p 3rd
    cd 3rd/
    git clone https://github.com/lukaszgemborowski/cpptoolbox.git
    cd cpptoolbox/
    git checkout a56eb3a41b76afcb53055fd2a0ba15ef1e2c3af4
    cd ../..
fi
