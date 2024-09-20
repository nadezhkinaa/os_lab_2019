#!/bin/bash

sum=0
len=$#

for i in "$@";
do
sum=$(($sum+$i))
done
sr_ar=$(($sum/$len))

echo "число аргументов: "$#
echo "ср арифметическое: " $sr_ar

