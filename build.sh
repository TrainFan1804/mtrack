#!/bin/bash

set -e

source scripts/pretty_print.sh

# ---------------------------
# Globals
# ---------------------------
CXX=g++
CXXFLAGS=""

# software source code specific paths (needed at buildtime)
SRC_DIR="./src"
INCLUDE_DIR="./include"
BUILD_DIR="./build"

QT_INCLUDE_DIR="/usr/include/qt6"

# Build options
BUILD_TEST=false
IS_CLEAN=false

VBASE=$(cat VERSION)
VSUFFIX=$(git rev-parse --abbrev-ref HEAD)
VERSION="$VBASE-$VSUFFIX"

# ---------------------------
# Functions
# ---------------------------

# Both "has_argument" and "extract_argument" are copied from
# https://medium.com/@wujido20/handling-flags-in-bash-scripts-4b06b4d0ed04
has_argument() {
    [[ ("$1" == *=* && -n ${1#*=}) || ( ! -z "$2" && "$2" != -*)  ]];
}

extract_argument() {
  echo "${2:-${1#*=}}"
}

function parse_args() {

    while [ $# -gt 0 ]; do
        case $1 in
            clean)
                IS_CLEAN=true
                ;;
            --dev)
                if [ "$BUILD_TYPE" = "release" ]; then
                    error "You can't set dev after set release"
                    exit 1
                fi
                BUILD_TYPE="dev"
                if has_argument $@; then
                    if [ $(extract_argument $@) = "test" ]; then
                        BUILD_TEST=true
                        shift
                    fi
                fi
                ;;
            --release)
                if [ "$BUILD_TYPE" = "dev" ]; then
                    error "You can't set release after set dev"
                    exit 1
                fi
                BUILD_TYPE="release"
                ;;
            *)
                error "Invalid option: $1"
                exit 1
                ;;
        esac
        shift
    done
}

function cleanup() {

    rm -rf **/$BUILD_DIR
    rm -f $INCLUDE_DIR/buildenv.h
    rm -rf $INCLUDE_DIR/gui/forms
    rm -f $SRC_DIR/gui/**/moc_*
    rm -f $SRC_DIR/gui/resources/qrc_*
}

function check_build_type() {

    if [ "$BUILD_TYPE" = "dev" ]; then
        CXXFLAGS="-g -O0 -Wall -DDEBUG"
        APPDATA_PATH="$PWD/dev/mtrack"
    elif [ "$BUILD_TYPE" = "release" ]; then
        APPDATA_PATH="$HOME/.local/share/mtrack"
    else
        error "Unknown build type: "$BUILD_TYPE""
        exit 1
    fi
}

function determine_qt_include_path() {

    # under debian path /usr/include/x86_64-linux-gnu will store qt6 headers
    if [ ! -d "$QT_INCLUDE_DIR" ]; then
        QT_INCLUDE_DIR="/usr/include/x86_64-linux-gnu/qt6"
        if [ ! -d "$QT_INCLUDE_DIR" ]; then
            error "Qt6 headers not found"
            exit 1
        fi
    fi
}

function pre_build() {

    local SCRIPT_DIR="./scripts"
    /bin/bash $SCRIPT_DIR/create_appdata.sh $APPDATA_PATH
    # this still cause trouble because the new created header will force the src files
    # that use this header to recompile even when there wasn't any change
    /bin/bash $SCRIPT_DIR/create_qtheader.sh
    /bin/bash $SCRIPT_DIR/create_qtresources.sh
}

function generate_moc_files() {

    local QT_MOC="/usr/lib/qt6/moc"
    # create moc files with .h files
    for wrapper_file in $(find $INCLUDE_DIR/gui/handler -name '*.h'); do
        wrapper_name="$(basename "${wrapper_file%.h}.cpp")"
        full_wrapper_name=$SRC_DIR/gui/handler/moc_$wrapper_name
        if [ $wrapper_file -nt $full_wrapper_name ]; then
            $QT_MOC $wrapper_file -o $full_wrapper_name
        fi
    done

    for wrapper_file in $(find $INCLUDE_DIR/gui/wrapper -name '*.h'); do
        wrapper_name="$(basename "${wrapper_file%.h}.cpp")"
        full_wrapper_name=$SRC_DIR/gui/wrapper/moc_$wrapper_name
        if [ $wrapper_file -nt $full_wrapper_name ]; then
            $QT_MOC $wrapper_file -o $full_wrapper_name
        fi
    done
}

function compile_source() {

    local QT_INCLUDES="-I$QT_INCLUDE_DIR -I$QT_INCLUDE_DIR/QtWidgets -I$QT_INCLUDE_DIR/QtCore -I$QT_INCLUDE_DIR/QtGui"
    local ALL_INCLUDES="-I$INCLUDE_DIR $QT_INCLUDES"

    local CPPFLAGS="-MMD -MP -fPIC -DVERSION="\"$VERSION\"" $ALL_INCLUDES"

    mkdir -p $BUILD_DIR 

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
            info "Compiling $src_file -> $obj_file"
            $CXX $CPPFLAGS $CXXFLAGS -c "$src_file" -o "$obj_file"
        fi
    done
}

function link_object() {

    local LNKFLAGS="-lsqlite3 -lQt6Widgets -lQt6Core -lQt6Gui"

    objs=$(find $BUILD_DIR -name '*.o')
    $CXX $objs -o "$BUILD_DIR/mtrack" $LNKFLAGS
}

# ---------------------------
# Main
# ---------------------------

parse_args $@
if $IS_CLEAN; then
    info "Start cleanup build artefacts..."
    cleanup
    success "Successfully cleanup build artefacts"
    if [ "$BUILD_TYPE" = "" ]; then
        exit 0
    fi
fi

check_build_type
determine_qt_include_path
info "Start building \"$BUILD_TYPE\" version $BOLD\"$VERSION\"$RESET$BLUE..."

pre_build
generate_moc_files
info "Start compiling source files..."
compile_source
success "Compiling source files successfully"

info "Start linking project..."
link_object
success "Linking project successfully"
success "Successfully build"

if $BUILD_TEST; then
    cd test
    info "Start building tests for version $BOLD\"$VERSION\"$RESET$BLUE..."
    /bin/bash ./build.sh $VERSION
    success "Successfully build test"
fi
