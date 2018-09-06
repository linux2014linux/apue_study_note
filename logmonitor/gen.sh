#!/bin/bash
rm -rf first.txt
g++ input.cpp -o input -lpthread

g++ output.cpp -o output -lpthread
#./input first.txt

g++ mvrm_test.cpp -o mvrm_test
