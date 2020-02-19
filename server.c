#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
void error(char *m)
{
        perror(m);
}

void doprocessing (int sock) {
   int n;
   char buffer[256];
   bzero(buffer,256);
   n = read(sock,buffer,255);
   
   if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }

   if (n < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }
        printf("Message received: %s\n",buffer);
        n = write(sock,"Message acknowledged",21);

}
int main(int argc, char *argv[])
{ int sockfd, newsockfd, port, clilen, n, pid;
        int c=2;
        char  buffer[256];
        struct sockaddr_in serv_addr, cli_addr;
        if (argc < 2)
                error("ERROR, no port provided\n");
        port = atoi(argv[1]);
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
                error("ERROR opening socket");

        memset((char *) &serv_addr, 0, sizeof(serv_addr));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(port); //host to network

        if (bind(sockfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
                error("ERROR binding to socket");

        listen(sockfd,3);
        clilen = sizeof(cli_addr);

        while(1){
        newsockfd = accept(sockfd,(struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0)
                 error("ERROR on accept");

        pid = fork();

        if(pid<0){
                error("ERROR on fork");
        }
        if(pid==0){
                close(sockfd);
		doprocessing(newsockfd);
                exit(0);
        }
        c--;
        }
        return 0;
}

