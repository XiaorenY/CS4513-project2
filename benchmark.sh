#!/bin/bash

# You will use sysbench to benchmark both your local machine (e.g., the CCC machines) and your Amazon Linux instance.

# Benchmark the CPU performance with the following:
sysbench --test=cpu --cpu-max-prime=20000 run

# There will be a lot of numeric output, but the most important is the total time (e.g., 3.5160s).
# Benchmark file I/O performance, by first creating a test file that is much bigger than your RAM (otherwise, you get memory performance). Check the RAM on your system with free -h and use at least twice this value for the file size. Say this is 16GB, then prepare files via:

sysbench --test=fileio --file-total-size=16G prepare
# Then, run the benchmark:

sysbench --test=fileio --file-total-size=16G --file-test-mode=rndrw --init-rng=on --max-time=30 --max-requests=0 run
# The important number is the Mb/sec value (e.g., 1.19779 Mb/sec).
# When done, delete the prepared files from your system:

sysbench --test=fileio --file-total-size=16G cleanup

