#!/bin/bash
export DEVKITPRO=/opt/devkitpro
mkdir -p build
cd build
cmake ..
make