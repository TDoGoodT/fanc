.PHONY: all clean

parser: clean
	flex scanner.lex
	bison -d parser.ypp --report=all
	g++ -std=c++17 -o hw3 *.cpp *.c

scanner: clean
	flex scanner.lex
	g++ -std=c++17 -o hw1 *.c

sanity1: scanner
	chmod +x hw1
	./hw1 < tests/sanity_test.txt

sanity2: parser
	chmod +x hw3
	./hw3 < tests/sanity_test.txt

sanity: sanity1 sanity2

clean:
	rm -f lex.yy.c
	rm -f parser.tab.*pp
	rm -f parser.output
	rm -f scanner.cpp
	rm -f hw2 hw1 hw3
	rm -f *.zip
	rm -rf selfcheck_tmp

submit: clean
	zip 313350035.zip *.cpp *.hpp parser.ypp scanner.lex Makefile README.md
	zip tests.zip tests/*.in tests/*.out
	bash selfcheck-hw3 313350035.zip tests.zip


