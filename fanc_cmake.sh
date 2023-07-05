#!/usr/bin/env bash
hn=$(hostname)
if [ "${hn}" = "snir-linux" ]
then
  /home/snir/.local/bin/cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=/home/snir/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/231.8109.222/bin/ninja/linux/x64/ninja -G Ninja -S /home/snir/wa/compi2/fanc -B /home/snir/wa/compi2/fanc/cmake-build-debug
  /home/snir/.local/bin/cmake --build /home/snir/wa/compi2/fanc/cmake-build-debug --target fanc -j 8
  /home/snir/.local/bin/cmake --build /home/snir/wa/compi2/fanc/cmake-build-debug --target fanc -j 8
  cp /home/snir/wa/compi2/fanc/cmake-build-debug/fanc /home/snir/wa/compi2/fanc/bin/fanc
elif [ "${hn}" = "snir-mac.local" ]
then
  "/Users/snir/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/231.8109.222/CLion.app/Contents/bin/cmake/mac/bin/cmake" -DCMAKE_BUILD_TYPE=Debug "-DCMAKE_MAKE_PROGRAM=/Users/snir/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/231.8109.222/CLion.app/Contents/bin/ninja/mac/ninja" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -G Ninja -S /Users/snir/WA/fanc -B /Users/snir/WA/fanc/cmake-build-debug
  "/Users/snir/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/231.8109.222/CLion.app/Contents/bin/cmake/mac/bin/cmake" --build /Users/snir/WA/fanc/cmake-build-debug --target fanc -j 8
  "/Users/snir/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/231.8109.222/CLion.app/Contents/bin/cmake/mac/bin/cmake" --build /Users/snir/WA/fanc/cmake-build-debug --target fanc -j 8
  cp /Users/snir/WA/fanc/cmake-build-debug/fanc /Users/snir/WA/fanc/bin/fanc
fi