#!/bin/bash
# change these per each homework
# link to tests: testsurl="https://webcourse.cs.technion.ac.il/fc159753hw_236360_202201/hw/WCFiles/X2363602022011-hw2-tests.zip"

makefileurl="https://webcourse.cs.technion.ac.il/fc159753hw_236360_202201/hw/WCFiles/X2363602022011-Makefile-hw2"
# number of tests:
numtests=2
# command to execute test:
command="./hw2 < t\$i.in >& t\$i.res"
hostname="snir-linux"
tmpdir="selfcheck_tmp"
if [ $( hostname ) != "$hostname" ]
then
echo "This script is only intended to run on "$hostname"!"
exit
fi
if [ -z "$1" ]
then
echo "Usage: $0 submission_file"
exit
fi
if [ ! -f "$1" ]
then
echo "Submission zip file not found!"
exit
fi rm -rf "$tmpdir" &> /dev/null
if [ -d "$tmpdir" ]
then
echo "Cannot clear tmp directory. Please delete '"$tmpdir"' manually and try again"
exit
fi
mkdir "$tmpdir" &> /dev/null
unzip "$1" -d "$tmpdir" &> /dev/null
if [[ $? != 0 ]]
then
echo "Unable to unzip submission file!"
exit
fi
cd "$tmpdir"
if [ ! -f scanner.lex ]
then
echo "File scanner.lex not found!"
exit
fi
if [ ! -f parser.ypp ]
then
echo "File parser.ypp not found!"
exit
fi
if [ ! -f expected.cpp ]
then
echo "File output.cpp not found!"
exit
fi
if [ ! -f expected.hpp ]
then
echo "File output.hpp not found!"
exit
fi
wget --no-check-certificate "$makefileurl" -O Makefile-hw2 &> /dev/null
if [ ! -f Makefile-hw2 ]
then
echo "Unable to download makefile!"
exit
fi
make -f Makefile-hw2
if [ ! -f hw2 ]
then
echo "Cannot build submission!"
exit
fi
wget --no-check-certificate "$testsurl" -O hw2-tests.zip &> /dev/null
if [ ! -f hw2-tests.zip ]
then
echo "Unable to download tests!"
exit
fi
unzip hw2-tests.zip &> /dev/null
if [[ $? != 0 ]]
then
echo "Unable to unzip tests!"
exit
fi
i="1"
while [ $i -le $numtests ]
do
eval $command
diff t$i.res t$i.out &> /dev/null
if [[ $? != 0 ]]
then
echo "Failed test #"$i"!"
exit
fi
i=$[$i+1]
done
cd - &> /dev/null
rm -rf "$tmpdir"
echo "Ok to submit :)"
exit
