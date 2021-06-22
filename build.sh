#! /bin/bash
g++ lexical.cpp syntax.cpp common.cpp codegen.cpp main.cpp -std=c++11 -o Compiler 

g++ interpret.cpp -std=c++11 -o Interpreter