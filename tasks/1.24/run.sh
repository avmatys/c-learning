#!/bin/bash

echo "---Compile the main---"

gcc -I ./../../libs/include -c main.c -o main.o
gcc main.o ./../../libs/build/types.o -o main.out

if [ $? -eq 0 ]
then
    echo "Run the program"
    echo "-----------------------"
    ./main.out
else
    echo "Error of linking"
fi
