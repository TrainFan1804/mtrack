#!/bin/bash

BUILD_TYPE=${1:-dev}
VERSION=$(git rev-parse --abbrev-ref HEAD)

# software source code specific paths (needed at buildtime)
SRC_DIR="src"
INCLUDE_DIR="include"
BUILD_DIR="build"

# appdata specific paths (needed at runtime)
APPDATA_PATH="$HOME/.local/share/mtrack"
LOG_PATH="$APPDATA_PATH/log"
PYTHON_PATH="$PWD/dist/pyenv/bin/python3"
GUI_PY="$PWD/gui/main.py"

if [ "$BUILD_TYPE" = "dev" ]; then
    CXXFLAGS="-g -O0 -Wall -DDEBUG"
elif [ "$BUILD_TYPE" = "release" ]; then
    CXXFLAGS=""
    if [ "$VERSION" = "main" ]; then
        VERSION=$(git describe --tags --abbrev=0)"-release"
    fi
else
    echo "Unknown build type "$BUILD_TYPE""
    exit 1
fi

echo "Start building \"$BUILD_TYPE\" version \"$VERSION\"..."

mkdir -p $BUILD_DIR 

if ! g++ $CXXFLAGS -DVERSION="\"$FULL_VERSION\"" -I$INCLUDE_DIR $(find $SRC_DIR -name '*.cpp') -o $BUILD_DIR/mtrack -lsqlite3; then
    echo "Build failed"
    exit 1
fi

mkdir -p $APPDATA_PATH
mkdir -p $LOG_PATH

# Because I am to lazy to define the path in the C++ code twice I came up with
# this crazy stuff...
# Maybe I should rewrite the entire programm in just a big script that execute a big cat command...
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

// response codes for the communication between frontend and backend
#define ASK_DATA        "100"
#define ADD_RESPONSE    "101"
#define RM_RESPONSE     "102"
#define SEND_RESPONSE   "200"
#define TRN_END         "300"
#define WRG_FORMAT      "400"
#define DB_ERROR        "500"

#endif
EOF

# pls forgive me for this...
cat << EOF > gui/responsecodes.py
ASK_DATA        =   "100"
ADD_RESPONSE    =   "101"
RM_RESPONSE     =   "102"
SEND_RESPONSE   =   "200"
TRN_END         =   "300"
WRG_FORMAT      =   "400"
DB_ERROR        =   "500"
EOF

echo "Successfully build"
