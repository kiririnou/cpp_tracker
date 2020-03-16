#!/bin/sh
set -e
cd cpp_tracker/
rm -rf build/*
cmake -B build/ -DCMAKE_TOOLCHAIN_FILE=scripts/win64.cmake
make -C build/ -j2
cp /cpp_tracker.zip /dist/cpp_tracker.zip
