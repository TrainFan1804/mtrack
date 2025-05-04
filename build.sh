#!/bin/bash

set -e

BUILD_TYPE=${1:-dev}
VERSION=$(git rev-parse --abbrev-ref HEAD)

if [ "$BUILD_TYPE" = "dev" ]; then
    CXXFLAGS="-g -O0 -Wall -DDEBUG"
    APPDATA_PATH="./dev/mtrack"
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

#endif
EOF

echo "Start building \"$BUILD_TYPE\" version \"$VERSION\"..."

# software source code specific paths (needed at buildtime)
CXX=g++
SRC_DIR="./src"
INCLUDE_DIR="./include"
BUILD_DIR="./build"
mkdir -p $BUILD_DIR 

QT_INCLUDES="-I/usr/include/qt -I/usr/include/qt/QtWidgets"
ALL_INCLUDES="-I$INCLUDE_DIR $QT_INCLUDES"

CPPFLAGS="-MMD -MP -fPIC -DVERSION="\"$VERSION\"" $ALL_INCLUDES"
LNKFLAGS="-lsqlite3 -lQt5Widgets -lQt5Core -lQt5Gui"

# stuff for Qt forms
FORM_DIR="./forms"
for form_file in $(find $FORM_DIR -name '*.ui'); do
    header_name="$(basename "${form_file%.ui}.h")"
    uic $form_file -o $INCLUDE_DIR/gui/$header_name
done

# Compile .cpp files to .o files 
for src_file in $(find $SRC_DIR -name '*.cpp'); do
    obj_file="$BUILD_DIR/"${src_file%.cpp}.o""
    dep_file="${obj_file%.o}.d"
    mkdir -p $(dirname $obj_file)

    needs_build=0

    # check if .o or .d are missing
    if [ ! -f "$obj_file" ] || [ ! -f "$dep_file" ]; then
        needs_build=1
    else
        # check if any dependency in .d is newer than .o file
        deps=$(sed -e 's/\\$//' "$dep_file" | cut -d ':' -f2- | tr ' ' '\n')
        for dep in $deps; do
            if [ "$dep" -nt "$obj_file" ]; then
                needs_build=1
                break
            fi
        done
    fi

    if [ "$needs_build" -eq 1 ]; then
        echo "Compiling $src_file -> $obj_file"
        $CXX $CPPFLAGS $CXXFLAGS -c "$src_file" -o "$obj_file"
    fi
done

echo "Linking files..."
objs=$(find $BUILD_DIR -name '*.o')
$CXX $objs -o "$BUILD_DIR/mtrack" $LNKFLAGS

echo "Successfully build"
