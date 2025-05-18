#!/bin/bash

# ---------------------------
# This script is part of /build.sh and shouldn't 
# be executed on it's own
# ---------------------------
set -e

CXXFLAGS="-g -O0 -Wall -DDEBUG"

# $1 := VERSION
echo "Start building tests for version \"$1\"..."

# software source code specific paths (needed at buildtime)
CXX=g++
TEST_SRC_DIR="./src"
TEST_INCLUDE_DIR="./include"
TEST_BUILD_DIR="./build"

PROD_INCLUDE_DIR="../include"

mkdir -p $TEST_BUILD_DIR 

QT_INCLUDES="-I/usr/include/qt6 -I/usr/include/qt6/QtWidgets -I/usr/include/qt6/QtCore -I/usr/include/qt6/QtGui"
ALL_INCLUDES="-I$TEST_INCLUDE_DIR -I$PROD_INCLUDE_DIR $QT_INCLUDES"

CPPFLAGS="-MMD -MP -fPIC $ALL_INCLUDES"
LNKFLAGS="-lsqlite3 -lQt6Widgets -lQt6Core -lQt6Gui"

# Compile .cpp files to .o files 
for src_file in $(find $TEST_SRC_DIR -name '*.cpp'); do
    obj_file="$TEST_BUILD_DIR/"${src_file%.cpp}.o""
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

test_objs=$(find $TEST_BUILD_DIR -name '*.o')

PROD_BUILD_DIR="../build"
prod_test_objs=$(find $PROD_BUILD_DIR -name '*.o' ! -name 'main.o')

$CXX $prod_test_objs $test_objs -o "$TEST_BUILD_DIR/mtrack_test" $LNKFLAGS

echo "Successfully build test"
