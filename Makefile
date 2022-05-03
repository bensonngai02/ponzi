run:
	g++ -ggdb -o run src/secd.cxx src/intermediateParser.cxx src/atom.cxx src/node.cxx src/expression.cxx
	./run

compile:
	g++ -ggdb -o compile src/compiler.cxx src/secd.cxx src/intermediateParser.cxx src/atom.cxx src/node.cxx src/expression.cxx
	./compile

clean:
	rm -rf ./run ./compile


CXX_FILES=${wildcard *.cxx}
CXX_O_FILES=${addprefix $B/,${subst .cxx,.o,${CXX_FILES}}}
LINK=${firstword ${patsubst %.cxx,${CXX},${CXX_FILES}}}

LINK_FLAGS=
TEST_FILES=${sort ${wildcard *.test}}
TESTS=${subst ${TEST_FILES}}
OK_FILES=${subst .ok,${TEST_FILES}}
OUT_FILES=${subst .out,${TEST_FILES}}
DIFF_FILES=${subst .diff,${TEST_FILES}}
RESULT_FILES=${subst .result,${TEST_FILES}}

test: Makefile ${TESTS}

$B/funcc: ${CXX_O_FILES} ${C_O_FILES}
	@mkdir -p $B
	${LINK} -o $@ ${CXX_O_FILES}
${CXX_O_FILES} : $B/%.o: %.cxx Makefile
	@mkdir -p $B
	${CXX} -MMD -MF $B/$*.d -c -o $@ ${CXX_FLAGS} $*.cxx
${TESTS}: %.test : Makefile %.result
	echo "$* ... $$(cat $*.result) [$$(cat $*.time)]"
${RESULT_FILES}: %.result : Makefile %.diff
	echo "unknown" > $@
	((test -s $*.diff && echo "fail") || echo "pass") > $@
${OUT_FILES}: %.out : Makefile %.arm %.args
	@echo "failed to run" > $@
	-time --quiet -f '%e' -o $*.time timeout 10 ./$*.arm `cat $*.args` > $@

-include $B/*.d

