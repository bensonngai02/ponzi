run: 
	g++ -ggdb -o run src/secd.cxx src/parser.cxx src/atom.cxx src/node.cxx src/expression.cxx
	./run tests/ponzi/math.ponzi

compile: 
	g++ -ggdb -o compile src/compiler.cxx src/secd.cxx src/parser.cxx src/atom.cxx src/node.cxx src/expression.cxx
	./compile tests/scheme/math.scheme

clean: 
	rm -rf ./run ./compile
