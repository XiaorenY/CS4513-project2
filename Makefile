# Makefile for distributed shell program

CC = gcc
CFLAGS = 
LIBFLAGS = -lcrypt

all: client server

server: server.c header.h
	$(CC) $(CFLAGS) server.c -o server $(LIBFLAGS)

client: client.c header.h
	$(CC) $(CFLAGS) client.c -o client $(LIBFLAGS)

clean:
	/bin/rm -rf *.o core client server



