#!/bin/bash

set -e

source scripts/pretty_print.sh

RES_DIR="./src/gui/resources"
mkdir -p $RES_DIR

info "Start building resources..."

if [ assets/resources.qrc -nt $RES_DIR/qrc_resources.cpp ]; then
    /usr/bin/rcc assets/resources.qrc -o $RES_DIR/qrc_resources.cpp
fi

success "Building resources successfully"
