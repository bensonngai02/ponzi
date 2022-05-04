#!/bin/bash
rm -rf Makefile
echo -e "run: \n\tg++ -ggdb -o run src/secd.cxx src/parser.cxx src/atom.cxx src/node.cxx src/expression.cxx\n\t./run tests/ponzi/$1.ponzi\n" >> Makefile

echo -e "compile: \n\tg++ -ggdb -o compile src/compiler.cxx src/secd.cxx src/parser.cxx src/atom.cxx src/node.cxx src/expression.cxx\n\t./compile tests/scheme/$1.scheme > tests/scheme/$1.out\n " >> Makefile

echo -e "clean: \n\trm -rf ./run ./compile" >> Makefile

