run: 
	g++ -ggdb -o run src/secd.cxx src/parser.cxx src/atom.cxx src/node.cxx src/expression.cxx
	./run tests/ponzi/multiplyTwo.ponzi

compile: 
	g++ -ggdb -o compile src/compiler.cxx src/secd.cxx src/parser.cxx src/atom.cxx src/node.cxx src/expression.cxx
	./compile tests/scheme/multiplyTwo.scheme > tests/scheme/multiplyTwo.out
 
clean: 
	rm -rf ./run ./compile tests/scheme/*.out tests/ponzi/*.out
