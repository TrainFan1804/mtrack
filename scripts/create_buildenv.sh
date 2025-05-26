#!/bin/bash

set -e

source scripts/pretty_print.sh

# $1 := APPDATA_PATH
if [ "$#" -ne 1 ]; then
    error "Something went wrong trying to build buildenv.h"
    exit 1
fi

if [ -f include/buildenv.h ]; then
    info "Skip buidling buildenv.h"
    exit 0
fi

info "Start building buildenv.h..."

LOG_PATH="$1/log"
BACKUP_PATH="$1/backup"

mkdir -p $1
mkdir -p $LOG_PATH
mkdir -p $BACKUP_PATH

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
#define LOG_DIR_PATH        "$LOG_PATH"
#define BACKUP_DIR_PATH     "$BACKUP_PATH" 

#endif
EOF

success "Building buildenv.h successfully"
