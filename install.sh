#!/bin/bash

set -e

./build.sh release
if [ $? -ne 0 ]; then
    echo "Abort installation because build failed!"
    exit 1
fi

echo "Start installing.."

# Installing mTrack will move the executable into the users bin directory
# to access the software via the terminal
SCRIPT_INSTALL_PATH="$HOME/.local/bin"
BUILD_TARGET="build/mtrack"

if [ ! -f "$BUILD_TARGET" ]; then
    echo "Build target not found"
    exit 1
fi

mkdir -p $SCRIPT_INSTALL_PATH

if cp $BUILD_TARGET "$SCRIPT_INSTALL_PATH/mtrack"; then
    echo "Successfully installed. Make sure to add \"$SCRIPT_INSTALL_PATH\" into your PATH"
else
    echo "Something went wrong"
    exit 1
fi
