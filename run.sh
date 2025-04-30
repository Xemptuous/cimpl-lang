#!/bin/sh
scriptDir=$(dirname -- "$(readlink -f -- "$BASH_SOURCE")")

cd scriptDir
cmake -B build -G Ninja
ninja -C build
$scriptDir/build/bin/cimpl
