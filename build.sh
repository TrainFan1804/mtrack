#!/bin/bash

set -e

BUILD_TYPE=${1:-dev}
VERSION=$(git rev-parse --abbrev-ref HEAD)

if [ "$BUILD_TYPE" = "dev" ]; then
    CXXFLAGS="-g -O0 -Wall -DDEBUG"
    APPDATA_PATH="$PWD/dev/mtrack"
    LOG_PATH="$APPDATA_PATH/log"
elif [ "$BUILD_TYPE" = "release" ]; then
    CXXFLAGS=""
    if [ "$VERSION" = "main" ]; then
        VERSION=$(git describe --tags --abbrev=0)"-release"
    fi
    APPDATA_PATH="$HOME/.local/share/mtrack"
    LOG_PATH="$APPDATA_PATH/log"
else
    echo "[ERROR]: Unknown build type "$BUILD_TYPE""
    exit 1
fi

# appdata specific paths (needed at runtime)
PYTHON_PATH="$PWD/build/pyenv/bin/python3"
GUI_PY="$PWD/gui/main.py"

if [ ! -d $PWD/build/pyenv ]; then
    echo "Creating python enviroment..."
    python3 -m venv $PWD/build/pyenv
fi

mkdir -p $APPDATA_PATH
mkdir -p $LOG_PATH

# Because I am to lazy to define the path in the C++ code twice I came up with
# this crazy stuff...
# Maybe I should rewrite the entire programm in just a big script that execute a big cat command...
# This is actually crazy stuff because mTrack will use the correct data path automatically
cat << EOF > include/buildenv.h
#ifndef BUILDENV_H
#define BUILDENV_H

/*
    mTrack use a different location for the appdata itself. Right now
    it's just a simple sqlite database and a (optional) log.
*/
#define APPDATA_DIR_PATH    "$APPDATA_PATH"
#define LOG_DIR_PATH        "$LOG_PATH"
#define PYTHON_PATH         "$PYTHON_PATH"
#define GUI_PY              "$GUI_PY"

#endif
EOF

echo "Start building \"$BUILD_TYPE\" version \"$VERSION\"..."

# software source code specific paths (needed at buildtime)
SRC_DIR="$PWD/src"
INCLUDE_DIR="$PWD/include"
BUILD_DIR="$PWD/build"
OBJ_DIR="$BUILD_DIR/obj"

mkdir -p $BUILD_DIR 
mkdir -p $OBJ_DIR

for file in $(find $SRC_DIR -name '*.cpp'); do
    obj="$OBJ_DIR/$(basename "$OBJ_DIR/${file%.cpp}.o")"
    if [ "$file" -nt "$obj" ]; then
        echo "Compiling $file -> $obj"
        if ! g++ -c $CXXFLAGS -DVERSION="\"$VERSION\"" -I$INCLUDE_DIR "$file" -o "$obj"; then
            echo "[ERROR]: Build failed"
            exit 1
        fi
    fi
done

g++ $OBJ_DIR/*.o -o "$BUILD_DIR/mtrack" -lsqlite3

echo "Successfully build"
