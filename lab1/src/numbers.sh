#!/bin/bash

for i in {1..150}; do 
echo $((RANDOM%1000))>> numbers.txt
done
