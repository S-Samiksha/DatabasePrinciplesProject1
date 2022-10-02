#!/bin/bash

g++ -o main include/*.cpp main.cpp 
./main > output.txt

exec bash