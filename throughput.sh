#!/bin/bash

# measure the maximum throughput from server to client.
# use > redirect stream into a file. 
# size of file divided by round-trip time is the throughput from server to client.
# test with file size 1M
throughputsum=0
for i in {0..49}
do
	sync
	time0=$(date +%s%3N)
	./client -c "cat test1M.dat" -s ec2-52-33-162-225.us-west-2.compute.amazonaws.com -p 5000 > test1M.dat
	sync
	time1=$(date +%s%3N)

	timediff=$((time1-time0))
	echo "RTT is $timediff milliseconds"

	filesize=$((8 * $(stat -c%s test1M.dat)))

	throughput=$((filesize*1000/timediff))
	throughputsum=$((throughputsum+throughput))

done

throughputavg=$((throughputsum/50))
echo "average throughput is $throughputavg bits per second"


