#!/bin/bash
# get input from user
test_dir=test
echo "Enter the name of the test"
read test_name
cp $test_dir/compiler/input/$test_name.in cmake-build-debug/test.in
