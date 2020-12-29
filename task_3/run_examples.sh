#!/bin/sh

make config=release
clear

for file in examples/*
do
	printf "testing file $file:\n"
	bin/Release-linux-x86_64/task_3 "$file"
	printf "\n=====================================================================================\n"
done
