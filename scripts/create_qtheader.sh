#!/bin/bash

# ---------------------------
# This script is part of /build.sh and shouldn't 
# be executed on it's own
# ---------------------------
set -e

source scripts/pretty_print.sh

# this still cause trouble because the new created header will force the src files
# that use this header to recompile even when there wasn't any change
FORM_INCLUDE_DIR=./include/gui/forms
mkdir -p $FORM_INCLUDE_DIR

# create .h files from .ui files
for form_file in $(find ./ -name '*.ui'); do
    header_name="$(basename "${form_file%.ui}.h")"
    full_header_name=$FORM_INCLUDE_DIR/$header_name
    if [ $form_file -nt $full_header_name ]; then
        /usr/lib/qt6/uic $form_file -o $full_header_name
    fi
done

success "Building Qt header forms successfully"
