# CS4513-project2
Basic Distributed Shell
Xiaoren Yang

source file include, "server.c", "client.c", "header.h" and "Makefile"

"server"
Usage: ./server -p <port> [-h]
	-p <port>	- port
	[-h] optional argument, ask for display usage
	[-t] tear down executation, used for experiments


"client"
Usage: ./client -c <command> -s <host> -p <port> [-h]
	-c <command>	- command asked to excecute
	-s <host>	- Internet name of server host
	-p <port>	- port
	[-h] optional argument, ask for display usage
	[-t] tear down executation, used for experiments

