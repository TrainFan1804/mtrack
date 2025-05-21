#!/bin/bash

set -e

# $1 := APPDATA_PATH
# $2 := LOG_PATH 
# $3 := BACKUP_PATH
# $4 := INCLUDE_DIR
if [ "$#" -ne 4 ]; then
    echo "Something went wrong trying to build buildenv.h"
    exit 1
fi

mkdir -p $1
mkdir -p $2
mkdir -p $3

if [ -f $4/buildenv.h ]; then
    echo "Skip buidling buildenv.h"
    exit 0
fi

echo "Start building buildenv.h..."

# Because I am to lazy to define the path in the C++ code twice I came up with
# this crazy stuff...
# Maybe I should rewrite the entire programm in just a big script that execute a big cat command...
# This is actually crazy stuff because mTrack will use the correct data path automatically
cat << EOF > include/buildenv.h
#ifndef BUILDENV_H
#define BUILDENV_H

/*
    mTrack use a different location for the appdata itself. Right now
    it's just a simple sqlite database and a (optional) log.
*/
#define APPDATA_DIR_PATH    "$1"
#define LOG_DIR_PATH        "$2"
#define BACKUP_DIR_PATH     "$3" 

#endif
EOF

echo "Building buildenv.h successfully"
