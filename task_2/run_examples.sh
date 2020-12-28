#!/bin/sh

make config=release

for file in examples/*
do
	printf "testing file $file:\n"
	bin/Release-linux-x86_64/task_2 "$file"
	printf "\n=====================================================================================\n"
done
