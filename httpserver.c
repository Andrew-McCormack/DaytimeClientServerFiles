#include "unp.h"
#include <time.h>
#include <string.h>

int main (int argc, char **argv)
{
     int listenfd, connfd, n;
     struct sockaddr_in servaddr;
     char buff[MAXLINE], recvline[MAXLINE];
     //char cmd[16];
     //char path[64];
     //char vers[16];
     //time_t ticks;

	 if(argc != 2)
		err_quit("usage: <Program Number> <Port No.>");

     listenfd = socket(AF_INET, SOCK_STREAM, 0);

     bzero(&servaddr, sizeof(servaddr));
     servaddr.sin_family = AF_INET;
     servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
     servaddr.sin_port = htons(atoi(argv[1]));

     Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

     Listen(listenfd, LISTENQ);

     for ( ; ; ) {
		connfd = accept(listenfd, (SA *) NULL, NULL);
		
		while((n = read(connfd, recvline, MAXLINE)) > 0) {//read while there is still data to read

			recvline[n] = 0;//null terminate the string
			if(fputs(recvline, stdout) == EOF)
				err_sys("fputs error");
			
			if((strstr(recvline, "\r\n\r\n")) != NULL)
				break;
		}

        snprintf(buff, sizeof(buff), "Hello\r\n");
        Write(connfd, buff, strlen(buff)); //write contents of buffer to confd socket
		
		getchar();
		Close(connfd);
     }
}
