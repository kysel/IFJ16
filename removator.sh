#!/bin/bash

cat borrowed.txt | while read line
do
   # do something with $line here
   echo "removing $line"
   rm odevzdat/test/input/*$line.*
done
exit 0