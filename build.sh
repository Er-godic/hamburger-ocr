#!/bin/sh

# build project

cd build
make

# license
cd ..

mkdir -p bin/doc/stb
cp stb/LICENSE bin/doc/stb/LICENSE
