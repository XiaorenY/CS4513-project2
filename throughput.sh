#!/bin/bash

# measure the maximum throughput from server to client.
# use > redirect stream into a file. 
# size of file divided by round-trip time is the throughput from server to client.
for j in {1..10}
do
	# test with file size 1M
	timesum=0
	throughputsum=0
	for i in {0..9}
	do
		sync
		time0=$(date +%s%3N)
		./client -c "cat test$j" -s ec2-52-33-162-225.us-west-2.compute.amazonaws.com -p 5000 > test$j
		sync
		time1=$(date +%s%3N)

		timediff=$((time1-time0))
		# echo "RTT is $timediff milliseconds"

		filesize=$((8 * $(stat -c%s test$j)))

		throughput=$((filesize*1000/timediff))
		timesum=$((timesum+timediff))
		echo "test$j: latency is $timediff"
		throughputsum=$((throughputsum+throughput))
		echo "test$j: throughput is $throughput"
	done

	timeavg=$((timesum/10))
	throughputavg=$((throughputsum/10))
	echo "test$j average latency is $timeavg milliseconds"
	echo "test$j average throughput is $throughputavg bits per second"

done

