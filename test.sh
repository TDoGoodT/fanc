#!/bin/bash

# Directory containing the test files
tests_dir="all_tests"

# Iterate over files with .in postfix
for full_name in "$tests_dir"/*.in.out.hw3.out; do
    # Remove the postfix keep the first work until .
    file_name=${full_name%.in.out.hw3.out}
    cp "$full_name" "$file_name.hw3.out" && rm $full_name
#    echo "Running test $file_name to $file_name.res"
#    ./hw5 < "$file_name" > "$file_name.res"
#
#    # Compare the output with the corresponding .out file
#    diff_output=$(diff "$file_name.res" "$file_name.out")
#
#    # Check if there are any differences
#    if [[ -z $diff_output ]]; then
#        echo "Test $file_name: PASSED"
#	rm "$file_name.res"
#	rm "$file_name.out"
#	rm "$file_name"
#    else
#        echo "Test $file_name: FAILED"
#        echo "Differences:"
#        echo "$diff_output"
#    fi
done

