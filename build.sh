#!/bin/sh

./bootstrap.sh
mkdir -p build
cd build
cmake ..
cmake --build .
cd ..