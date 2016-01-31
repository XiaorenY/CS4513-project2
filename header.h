#ifndef HEADER_H
#define HEADER_H

#define USERNAME "abc"
#define PASSWORD "cba"
#define BUFFSIZE 1024

int writeSocket(int sock, void *writeBuf, int len) {
	int bytesWrite;
	if((bytesWrite = write(sock, writeBuf, len)) != len){
		perror("write() failed");
		exit(len);
	}
	return bytesWrite;
}

int readSocket(int sock, void *readBuf, int len) {
	int n, bytesRead = 0;

	while (bytesRead < len) {
		if((n = read(sock, readBuf + bytesRead, len - bytesRead)) <= 0) {
			perror("read() failed");
			exit(len);
		}
		bytesRead += n;
	}
	return bytesRead;
}



#endif
