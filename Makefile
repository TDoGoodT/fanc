.PHONY: all clean

parser: clean
	flex scanner.lex
	bison -d parser.ypp --report=all
	g++ -std=c++17 -o bin/fanc *.cpp *.c
	chmod +x bin/fanc

parser-cmake:
	/home/snir/.local/bin/cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=/home/snir/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/231.8109.222/bin/ninja/linux/x64/ninja -G Ninja -S /home/snir/wa/compi2/fanc -B /home/snir/wa/compi2/fanc/cmake-build-debug
	/home/snir/.local/bin/cmake --build /home/snir/wa/compi2/fanc/cmake-build-debug --target fanc -j 8
	/home/snir/.local/bin/cmake --build /home/snir/wa/compi2/fanc/cmake-build-debug --target fanc -j 8
	cp /home/snir/wa/compi2/fanc/cmake-build-debug/fanc /home/snir/wa/compi2/fanc/bin/fanc

scanner: clean
	flex scanner.lex
	g++ -std=c++17 -o hw1 *.c

sanity1: scanner
	chmod +x hw1
	./hw1 < tests/sanity_test.txt

sanity2: parser
	chmod +x fanc
	./fanc < tests/sanity_test.txt

sanity: sanity1 sanity2

clean:
	rm -f lex.yy.c
	rm -f parser.tab.*pp
	rm -f parser.output
	rm -f scanner.cpp
	rm -f hw2 hw1 fanc
	rm -f *.zip
	rm -rf selfcheck_tmp

submit: clean
	zip 313350035.zip *.cpp *.hpp parser.ypp scanner.lex Makefile README.md


