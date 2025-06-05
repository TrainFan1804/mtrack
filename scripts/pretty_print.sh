#!/bin/bash

# ---------------------------
# Colors and printing
# ---------------------------
RED="\033[31m"
GREEN="\033[32m"
BLUE="\033[34m"

BOLD="\033[1m"

RESET="\033[0m"

function info() {
    echo -e "$BLUE [INFO] $1 $RESET"
}

function success() {
    echo -e "$GREEN [SUCCESS] $1 $RESET"
}

function error() {
    echo -e "$RED [ERROR] $1 $RESET" >&2
}
