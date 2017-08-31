#!/bin/sh

bison LogicalParser.y
flex LogicalParser.l
g++ --std=c++14 -Wall -g LogicalParser.lex.cpp LogicalParser.tab.cpp ExpTree.cpp simplify.cpp step1.cpp step2.cpp main.cpp -o parser
