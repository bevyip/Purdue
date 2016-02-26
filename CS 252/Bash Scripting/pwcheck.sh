#!/bin/bash

#DO NOT REMOVE THE FOLLOWING TWO LINES
git add $0 >> .local.git.out
git commit -a -m "Lab 2 commit" >> .local.git.out

#Your code here


password=$( cat $1 )

NUM1=${#password}
NUM2=6
NUM3=32

strength=0

if [ $NUM1 -lt $NUM2 ]; then
	echo "Error: Password length invalid"
	exit
elif [ $NUM1 -gt $NUM3 ]; then
	echo "Error: Password length invalid"
	exit
fi

strength=$NUM1

if egrep -q [#$\+%@] $1; then
	let strength=strength+5
fi

if egrep -q [0-9] $1; then
	let strength=strength+5
fi

if egrep -q [A-Za-z] $1; then
	let strength=strength+5
fi

if egrep -q '([A-Za-z0-9])\1+' $1; then
	let strength=strength-10
fi

if egrep -q '[a-z][a-z][a-z]' $1; then
	let strength=strength-3
fi

if egrep -q '[A-Z][A-Z][A-Z]' $1; then
	let strength=strength-3
fi

if egrep -q '[0-9][0-9][0-9]' $1; then
	let strength=strength-3
fi

echo "Password Score: $strength"
