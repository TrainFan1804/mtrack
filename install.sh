#!/bin/bash

set -e

source scripts/pretty_print.sh

./build.sh release
if [ $? -ne 0 ]; then
    error "Abort installation because build failed!"
    exit 1
fi

info "Start installing..."

# Installing mTrack will move the executable into the users bin directory
# to access the software via the terminal
SCRIPT_INSTALL_PATH="$HOME/.local/bin"
BUILD_TARGET="$PWD/build/mtrack"

if [ ! -f "$BUILD_TARGET" ]; then
    error "Build target not found"
    exit 1
fi

mkdir -p $SCRIPT_INSTALL_PATH

if cp $BUILD_TARGET "$SCRIPT_INSTALL_PATH/mtrack"; then
    success "Successfully installed. Make sure to add \"$SCRIPT_INSTALL_PATH\" into your PATH"
else
    error "Something went wrong"
    exit 1
fi
