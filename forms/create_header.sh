#!/bin/bash

set -e

FORM_INCLUDE_DIR=../include/gui/forms
mkdir -p $FORM_INCLUDE_DIR
# create .h files from .ui files
for form_file in $(find ./ -name '*.ui'); do
    header_name="$(basename "${form_file%.ui}.h")"
    uic $form_file -o $FORM_INCLUDE_DIR/$header_name
done
