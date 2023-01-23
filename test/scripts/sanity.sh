#!/bin/bash
cd ~/technion/compi/analyzer/test/ || exit
function run_sanity() {
  executable=$1
  expected_file="expected/$executable/sanity.in.out"
  result_file="sanity.$executable.res"
  input_file="input/sanity.in"
  diff_file="sanity.$executable.diff"
  if [ -f "$executable" ]; then
      chmod a+x "$executable"
  else
    echo "File $executable not found!"
    exit
  fi
  echo "running test: $input_file";
  cat "$input_file"
  "./$executable" < "$input_file" > "$result_file";
  if [ -f "$expected_file" ]; then
    if cmp -s "$result_file" "$expected_file"; then
      echo "test passed";
      rm -rf "$executable"
      rm -rf ./*.res
    else
      echo "test failed";
      # write the result and expected expected in table format to make it easier to compare
      paste -d '|' "$result_file" "$expected_file" | column -t -s '|' > "$diff_file"
    fi;
  else
    echo "no expected output file for input file: $input_file";
  fi;


}
echo "running $1 sanity test"
# if first arg is "parser" then run parser simple test
run_sanity "$1"
