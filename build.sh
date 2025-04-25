#!/bin/bash

BUILD_TYPE=${1:-dev}
VERSION=$(git rev-parse --abbrev-ref HEAD)

if [ "$BUILD_TYPE" = "dev" ]; then
    CXXFLAGS="-g -O0 -Wall -DDEBUG"
elif [ "$BUILD_TYPE" = "release" ]; then
    CXXFLAGS=""
    if [ "$VERSION" = "main" ]; then
        VERSION=$(git describe --tags --abbrev=0)"-release"
    fi
else
    echo "[ERROR]: Unknown build type "$BUILD_TYPE""
    exit 1
fi

chmod +x $PWD/dist/bin/createbuildenv.sh
$PWD/dist/bin/createbuildenv.sh

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
