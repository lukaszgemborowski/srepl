#!/bin/sh

clone_github() {
    if [ ! -d 3rd/$2 ]; then
        mkdir -p 3rd/$2
        cd 3rd
        git clone https://github.com/$1/$2.git
        cd $2
        git checkout $3
        cd ../..
    fi
}

clone_github lukaszgemborowski cpptoolbox a56eb3a41b76afcb53055fd2a0ba15ef1e2c3af4
clone_github lukaszgemborowski args cd9ee23c233554d4d8722f958e7339772dc00b18
