#!/bin/sh
set -e

rm -rf build/*
cmake -B build/ -DCMAKE_TOOLCHAIN_FILE=scripts/win64.cmake
make -C build/ -j2
#cp /cpp_tracker.zip /dist/cpp_tracker.zip
