#!/bin/bash

if [ "$1" = "test" ]; then
    ./test/build/mtrack_test
    exit 0
fi

./build/mtrack "$@"
