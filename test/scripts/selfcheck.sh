#!/bin/bash
#	link to tests:
testsurl="https://webcourse.cs.technion.ac.il/fc159753hw_236360_202201/hw/WCFiles/hw1-tests.zip"

tmpdir="selfcheck_tmp"


rm -rf "$tmpdir" &> /dev/null
if [ -d "$tmpdir" ]
	then
		echo "Cannot clear tmp directory. Please delete '"$tmpdir"' manually and try again"
		exit
fi
mkdir "$tmpdir" &> /dev/null

cp ../scanner.lex "$tmpdir"
cp ../hw1.cpp "$tmpdir"
cp ../tokens.hpp "$tmpdir"
cp *.in "$tmpdir" 
cp *.out "$tmpdir" 

cd "$tmpdir"
if [ ! -f scanner.lex ]
	then
		echo "File scanner.lex not found!"
		exit
fi

flex scanner.lex &> /dev/null
if [[ $? != 0 ]] 
	then
		echo "Cannot build submission!"
		exit
fi
g++ -std=c++17 lex.yy.c hw1.cpp -o hw1.out &> /dev/null
if [[ $? != 0 ]] 
	then
		echo "Cannot build submission!"
		exit
fi
if [ ! -f hw1.out ]
	then
		echo "Cannot build submission!"
		exit
fi




#	command to execute test:
command="./hw1.out < t\$i.in >& t\$i.res"
i="1"
pass=1
while [ $i -le $numtests ]
	do
		eval $command
		diff t$i.res t$i.out &> /dev/null
		if [[ $? != 0 ]]; then
			echo "Failed test #"$i"!"
			pass=0
		else 
			rm t$i.res
		fi
		i=$[$i+1]
done

cd - &> /dev/null
if [ pass == 0 ]; then
	rm -rf "$tmpdir"
	echo "Ok to submit :)"
fi

exit
