#!/bin/bash

make bcd.o
cp -f bcd.o ./tests/bcd.o
cp -f bcd.h ./tests/bcd.h
cd ./tests
make 
./test_tool
make clean
rm bcd.o bcd.h bcd.c