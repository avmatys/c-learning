#!/bin/bash

mkdir -p ./../build

echo "Compiling types.c..."

gcc -I ./../include -c ./../src/types.c -o ./../build/types.o

if [ $? -eq 0 ]; then
    echo "OK"
else 
    echo "Failed"
    exit 1
fi
