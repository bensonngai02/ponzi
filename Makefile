run:
	g++ -ggdb -o run src/secd.cxx src/intermediateParser.cxx src/atom.cxx src/node.cxx src/expression.cxx
	./run

clean:
	rm -rf ./run
