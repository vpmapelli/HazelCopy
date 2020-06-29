#!/bin/bash

if [ "$1" != "" ]; then
  echo "$1"
  mkdir -p build
  cd build
  cmake .. "-DCMAKE_BUILD_TYPE=$1"
else
  echo "You have to define a CONFIGURATION as argument [Debug|Release]"
fi
