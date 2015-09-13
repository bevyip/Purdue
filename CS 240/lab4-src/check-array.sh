#!/bin/bash

sym=`grep -c '\[' array.c`
echo $sym
if [ "$sym" != "0" ]; then
    echo "DO NOT USE ARRAYS IN array.c USE POINTERS";
    grep -c '\[' mystring.c
    exit 1
else
    echo OK. No array operations;
    exit 0
fi


