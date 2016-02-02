#!/bin/bash

# measure time to setup connection, authentication. 
# use option -t to tear down the exec() call
timesum=0
for i in {0..49}
do
	sync
	time0=$(date +%s%3N)
	./client -c "ls -l" -s ec2-52-33-162-225.us-west-2.compute.amazonaws.com -p 5000 -t
	sync
	time1=$(date +%s%3N)

	timediff=$((time1-time0))
	echo "latency time is $timediff milliseconds"
	timesum=$((timesum+timediff))

done

timeavg=$((timesum/50))
echo "average latency is $timeavg milliseconds"

