#!/bin/bash

# generate 10 test file size range from 1M to 10M
unitSize=1048576
for i in {1..10}
do
	base64 /dev/urandom | head -c $((unitSize*i)) > test$i
done

