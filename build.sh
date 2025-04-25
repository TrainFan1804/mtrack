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

echo "Start building \"$BUILD_TYPE\" version \"$VERSION\"..."

# software source code specific paths (needed at buildtime)
SRC_DIR="src"
INCLUDE_DIR="include"
BUILD_DIR="build"
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

# appdata specific paths (needed at runtime)
APPDATA_PATH="$HOME/.local/share/mtrack"
LOG_PATH="$APPDATA_PATH/log"
PYTHON_PATH="$PWD/dist/pyenv/bin/python3"
GUI_PY="$PWD/gui/main.py"

mkdir -p $APPDATA_PATH
mkdir -p $LOG_PATH

# Because I am to lazy to define the path in the C++ code twice I came up with
# this crazy stuff...
# Maybe I should rewrite the entire programm in just a big script that execute a big cat command...
# I realized this can only be used in development because I couldn't compile this
# when there is no buidenv.h...
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
#define ASK_DATA            "100"
#define ADD_RESPONSE        "101"
#define RM_RESPONSE         "102"
#define SEND_RESPONSE       "200"
#define TRN_END             "300"
#define SEND_ID             "301"
#define WRG_FORMAT          "400"
#define DB_ERROR            "500"

#define RESPONSE_CODE_SIZE  4   // prefix = 3, . = 1

// macros for SQL queries
#define TABLE_NAME          "MEDIA"
#define TABLE_INT_COL       "rating"    // all table columns that are integer (important for casting from json to int)
#define TABLE_STR_COL       "name", "state", "type"      // all table columns that are string
#define TABLE_ALL_COL       "ID", "NAME", "RATING", "STATE", "TYPE"

const std::string SQL_JSON_SELECT_ALL = "SELECT json_group_array(json_object('id', ID, 'name', NAME, 'rating', RATING, 'state', STATE, 'type', TYPE)) AS json_result FROM " + std::string(TABLE_NAME) + ";";

const std::string SQL_CREATE = "CREATE TABLE IF NOT EXISTS " + std::string(TABLE_NAME) + "(ID INTEGER PRIMARY KEY, NAME TEXT NOT NULL, RATING INT NOT NULL, STATE TEXT NOT NULL, TYPE TEXT NOT NULL);";

#endif
EOF

mkdir -p "$PWD/gui/config"

# pls forgive me for this...
cat << EOF > gui/config/responsecodes.py
ASK_DATA        =   "100"
ADD_RESPONSE    =   "101"
RM_RESPONSE     =   "102"
SEND_RESPONSE   =   "200"
TRN_END         =   "300"
SEND_ID         =   "301"
WRG_FORMAT      =   "400"
DB_ERROR        =   "500"

RESPONSE_CODE_SIZE  =   3 + 1
EOF

cat << EOF > gui/config/mediasection.py
JSON_ID             =   'id'
NAME_LIST           =   ['name', 'rating', 'state', 'type']
EOF

echo "Successfully build"
