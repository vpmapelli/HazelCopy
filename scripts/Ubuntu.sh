#!/bin/bash

if [ "$1" != "" ]; then
  echo "$1"
  mkdir -p build
  cd build
  cmake .. "-DCMAKE_BUILD_TYPE=$1" -DCMAKE_EXPORT_COMPILE_COMMANDS=1
else
  echo "You have to define a CONFIGURATION as argument [Debug|Release]"
fi
