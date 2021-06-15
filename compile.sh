#!/bin/bash

SRC=$2
OUTPUT=$1

echo "SRC=$SRC"
echo "OUTPUT=$OUTPUT"

gcc -o $OUTPUT  -Wall $SRC utils.c z3_solver.c libz3.so -lpthread -w
