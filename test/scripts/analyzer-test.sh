#!/bin/bash
cd ~/technion/compi/analyzer/test/ || exit
cp ../analyzer .
input_path="input/"
expected_path="expected/analyzer/"
analyzer_path="./analyzer"
rm ./*.diff
rm ./*.res
rm ./*.sh

if [ -f "$analyzer_path" ]
then
    chmod a+x "$analyzer_path"
else
  echo "File analyzer not found at $analyzer_path!"
  exit
fi
echo "Running Tests"
for file in "$input_path"/*.in; do
  file=$(basename -- "$file")
  # shellcheck disable=SC2059
  printf "$file -- ";
  expected="$expected_path/${file}.out"
  result="${file%.in}.res"
  eval "$analyzer_path < $input_path/$file 2>&1 > $result";
  if [ -f "$expected" ]; then
    if cmp -s "$result" "$expected"; then
      echo -e " \e[42m  \e[0m";
      rm -rf "$result";
    else
      echo -e " \e[41m  \e[0m";
      # write the result and expected expected in table format to make it easier to compare
      touch "${file%.in}.diff"
      paste -d '|' "$result" "$expected" | column -t -s '|' > "${file%.in}.diff"
      printf "#!/bin/bash\n ./analyzer < input/%s > %s.res" "$file" "${file%.in}" > "${file%.in}cmd.sh"
    fi;
  else
    echo -e " \e[41mno file $expected\e[0m";
  fi;
done

