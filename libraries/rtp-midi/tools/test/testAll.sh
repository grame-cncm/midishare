#!/bin/bash

for file in *.test
do

  ./testrtpmidi $file > /dev/null
  if [ $? != 0 ]; then
      echo $file
      ./testrtpmidi $file
      exit 1
  fi;

done  
