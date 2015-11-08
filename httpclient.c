#include "unp.h"

int main (int argc, char **argv) {
	int sockfd, n;
	char recvline[MAXLINE + 1];
	char sendline[MAXLINE + 1];
	struct sockaddr_in servaddr; // special structure to pass info to TCP layer, defined in unp.h 

	if (argc != 3) // Check to see if 3 arguments were passed
		err_quit("usage: a.out <IPaddress>");

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) // return value of socket binded to sockfd, negative number means error
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr)); // bzero iniatilses a structure to all zeros, very old c function
	servaddr.sin_family = AF_INET; // Specifies the socket API to use TCP
	servaddr.sin_port = htons(atoi(argv[2])); /* daytime server port taken from the command line, hton reorders endian from host to network */
	if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) // sin_addr holds the IP address of server
		err_quit("inet_pton error for %s", argv[1]);

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0) // (sockfd that we created, address of the struct holding addressing info, size of struct) 
		err_sys("connect error");
	
	
	snprintf(sendline, sizeof(sendline), "GET /index.html HTTP/1.1\r\nHost: www.comp.dit.ie\r\nConnection: close\r\n\r\n"); // http conversation, stick into buffer than writes contents
	Write(sockfd, sendline, strlen(sendline)); 

	while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0; /* null terminate */
		if (fputs(recvline, stdout) == EOF) /* print data (time and date) returned from server */
			err_sys("fputs error");
	}

	if (n < 0)
		err_sys("read error");

	exit(0);
}
