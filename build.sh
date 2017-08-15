#!/bin/sh

bison LogicalParser.y
flex LogicalParser.l
g++ --std=c++14 -Wall -g LogicalParser.lex.cpp LogicalParser.tab.cpp main.cpp -o parser
