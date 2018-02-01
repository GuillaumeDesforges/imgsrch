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

cd "$EXEC_DIR"
