#!/bin/bash

BUILD_TYPE=${1:-dev}
VERSION="0.1.0"

SRC_DIR="src"
INCLUDE_DIR="include"
BUILD_DIR="build"

if [ "$BUILD_TYPE" = "dev" ]; then
    CXXFLAGS="-g -O0 -Wall -DDEBUG"
elif [ "$BUILD_TYPE" = "release" ]; then
    CXXFLAGS=""
else
    CXXFLAGS="-Wall"
fi

echo "Start building \"$BUILD_TYPE\" version \"$VERSION\"..."

if ! g++ $CXXFLAGS -DVERSION="\"$VERSION\"" -I$INCLUDE_DIR $(find $SRC_DIR -name '*.cpp') -o $BUILD_DIR/mtrack -lsqlite3; then
    echo "Build failed"
    exit 1
fi

echo "Successfully build"
