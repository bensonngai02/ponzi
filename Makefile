run:
	g++ -ggdb -o run src/secd.cxx src/intermediateParser.cxx src/atom.cxx src/node.cxx src/expression.cxx
	./run

compile:
	g++ -ggdb -o compile src/compiler.cxx src/secd.cxx src/intermediateParser.cxx src/atom.cxx src/node.cxx src/expression.cxx
	./compile

clean:
	rm -rf ./run ./compile
