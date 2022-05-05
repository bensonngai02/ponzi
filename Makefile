run: 
	g++ -ggdb -o run src/secd.cxx src/parser.cxx src/atom.cxx src/node.cxx src/expression.cxx
<<<<<<< HEAD
	./run tests/ponzi/basic.ponzi

compile: 
	g++ -ggdb -o compile src/compiler.cxx src/secd.cxx src/parser.cxx src/atom.cxx src/node.cxx src/expression.cxx
	./compile tests/scheme/basic.scheme

=======
	./run tests/ponzi/michael.ponzi

compile: 
	g++ -ggdb -o compile src/compiler.cxx src/secd.cxx src/parser.cxx src/atom.cxx src/node.cxx src/expression.cxx
	./compile tests/scheme/michael.scheme > tests/scheme/michael.out
 
>>>>>>> 210208d9e176bfdd6bec0db4f6ba87463d7b9c84
clean: 
	rm -rf ./run ./compile tests/scheme/*.out tests/ponzi/*.out
