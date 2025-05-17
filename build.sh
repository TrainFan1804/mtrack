#!/bin/bash

set -e

echo $SCRIPT_DIR

BUILD_TYPE=$1
VBASE=$(cat VERSION)
VSUFFIX=$(git rev-parse --abbrev-ref HEAD)
VERSION="$VBASE-$VSUFFIX"

if [ "$BUILD_TYPE" = "dev" ]; then
    CXXFLAGS="-g -O0 -Wall -DDEBUG"
    APPDATA_PATH="$PWD/dev/mtrack"
    LOG_PATH="$APPDATA_PATH/log"
elif [ "$BUILD_TYPE" = "release" ]; then
    CXXFLAGS=""
    APPDATA_PATH="$HOME/.local/share/mtrack"
    LOG_PATH="$APPDATA_PATH/log"
else
    echo "[ERROR]: Unknown build type: "$BUILD_TYPE""
    exit 1
fi

echo "Start building \"$BUILD_TYPE\" version \"$VERSION\"..."

# software source code specific paths (needed at buildtime)
CXX=g++
SRC_DIR="./src"
INCLUDE_DIR="./include"
BUILD_DIR="./build"

/bin/bash $INCLUDE_DIR/create_buildenv.sh $APPDATA_PATH $LOG_PATH $INCLUDE_DIR

mkdir -p $BUILD_DIR 

QT_INCLUDES="-I/usr/include/qt6 -I/usr/include/qt6/QtWidgets -I/usr/include/qt6/QtCore -I/usr/include/qt6/QtGui"
ALL_INCLUDES="-I$INCLUDE_DIR $QT_INCLUDES"

CPPFLAGS="-MMD -MP -fPIC -DVERSION="\"$VERSION\"" $ALL_INCLUDES"
LNKFLAGS="-lsqlite3 -lQt6Widgets -lQt6Core -lQt6Gui"

# stuff for Qt forms

# this still cause trouble because the new created header will force the src files
# that use this header to recompile even when there wasn't any change
FORM_DIR="./forms"
FORM_INCLUDE_DIR=$INCLUDE_DIR/gui/forms
mkdir -p $FORM_INCLUDE_DIR
# create .h files from .ui files
for form_file in $(find $FORM_DIR -name '*.ui'); do
    header_name="$(basename "${form_file%.ui}.h")"
    full_header_name=$FORM_INCLUDE_DIR/$header_name
    if [ $form_file -nt $full_header_name ]; then
        /usr/lib/qt6/uic $form_file -o $full_header_name
    fi
done

# create moc files with .h files
for wrapper_file in $(find $INCLUDE_DIR/gui/wrapper -name '*.h'); do
    wrapper_name="$(basename "${wrapper_file%.h}.cpp")"
    full_wrapper_name=$SRC_DIR/gui/wrapper/moc_$wrapper_name
    if [ $wrapper_file -nt $full_wrapper_name ]; then
        /usr/lib/qt6/moc $wrapper_file -o $full_wrapper_name
    fi
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

if [ "$2" = "test" ]; then
    cd test
    /bin/bash ./build.sh $BUILD_TYPE $VERSION
fi
