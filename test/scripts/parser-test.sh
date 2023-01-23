#!/bin/bash
cd ~/technion/compi/analyzer/test/ || exit
input_path="input/"
expected_path="expected/parser/"
parser_path="parser/"
if [ -f "$parser_path" ]
then
    chmod a+x "$parser_path"
else
  echo "File parser not found at $parser_path!"
  exit
fi
for file in "$input_path"/*.in; do
  echo "running test: $file";
  expected="$expected_path/${file%.in}.out"
  result="${file%.in}.res"
  $parser_path < "$input_path/$file" > "$result";
  if [ -f "$expected" ]; then
    if cmp -s "$result" "$expected"; then
      echo "test passed";
      rm -rf ./*.res
    else
      echo "test failed";
      # write the result and expected expected in table format to make it easier to compare
      paste -d '|' "$result" "$expected" | column -t -s '|' > "${file%.in}.diff"
      printf "#!/bin/bash\n ../parser < %s > %s.res" "$file" "${file%.in}" > "${file%.in}cmd.sh"
    fi;
  else
    echo "no expected output file for input file: $file";
  fi;
done

