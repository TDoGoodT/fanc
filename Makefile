.PHONY: all clean
all: clean-all analyzer test

flex-scanner:
	flex scanner.lex
	mv lex.yy.c lex.yy.cpp

bison-analyzer:
	bison  -d	analyzer.ypp.txt -o analyzer.tab.cpp
	#bison  -d	analyzer.ypp -o analyzer.tab.cpp

pre-build: bison-analyzer flex-scanner
analyzer: clean clean-test pre-build
	g++ -std=c++17 -o analyzer 	*.cpp *.hpp
	chmod 777 ./analyzer
	cp analyzer test/analyzer
	cd test && ./scripts/sanity.sh analyzer

clean-all: clean clean-test

clean:
	rm -f parser scanner analyzer
	rm -f lex.yy.*
	rm -f *.tab.*pp
	rm -f *.zip

clean-test:
	rm -f test/parser
	rm -f test/analyzer
	rm -f test/scanner
	rm -f test/*.res
	rm -f test/*.report
	rm -f test/*.diff
	rm -f test/*cmd.sh

submit: clean
	zip 000000000-313350035.zip *.ypp *.cpp *.hpp *.lex *.md

scanner-test: scanner
	echo "not implemented yet"

parser-test: clean-test parser
	cd test && ./parser-test.sh

test: clean-test  scanner-test parser-test

