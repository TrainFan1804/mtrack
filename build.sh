#!/bin/bash

BUILD_TYPE=${1:-dev}
VERSION=$(git describe  --tags --abbrev=0)

SRC_DIR="src"
INCLUDE_DIR="include"
BUILD_DIR="build"

if [ "$BUILD_TYPE" = "dev" ]; then
    CXXFLAGS="-g -O0 -Wall -DDEBUG"
    VERSION_SUFFIX="-dev"
elif [ "$BUILD_TYPE" = "release" ]; then
    CXXFLAGS=""
    VERSION_SUFFIX=""
else
    echo "Unknown build type "$BUILD_TYPE""
    exit 1
fi

FULL_VERSION="$VERSION$VERSION_SUFFIX"

echo "Start building \"$BUILD_TYPE\" version \"$FULL_VERSION\"..."

mkdir -p $BUILD_DIR 

if ! g++ $CXXFLAGS -DVERSION="\"$FULL_VERSION\"" -I$INCLUDE_DIR $(find $SRC_DIR -name '*.cpp') -o $BUILD_DIR/mtrack -lsqlite3; then
    echo "Build failed"
    exit 1
fi

echo "Successfully build"
