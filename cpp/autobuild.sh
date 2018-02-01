#!/bin/bash
set -e

EXEC_DIR="$( pwd )"
SOURCE_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
mkdir -p "$SOURCE_DIR/build"
cd "$SOURCE_DIR/build"

cmake ..
make
make doc
make test

while test $# -gt 0
do
    case "$1" in
        --clean) rm -r gen CMakeDoxyfile.in CMakeFiles CTestTestfile.cmake Makefile CMakeCache.txt CMakeDoxygenDefaults.cmake cmake_install.cmake imgsrchPYTHON_wrap.cxx Testing
            ;;
        --help) echo "Project autobuild, use --clean option to clean build directory"
            ;;
    esac
    shift
done

cd "$EXEC_DIR"
