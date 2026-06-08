#!/bin/bash

g++ -O3 main.cpp PathNode.cpp Algo.cpp -o test && ./test && rm ./test
