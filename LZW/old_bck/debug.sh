#!/bin/bash

cd ../JCInterface; make; cd ../LZW
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../JCInterface
gcc debug.c -g -Wall -Wextra -lcompression -L../JCInterface -o exq_debug
./exq_debug
rm exq_debug
# valgrind --leak-check=full -v ./exq_debug