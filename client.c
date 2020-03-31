//
// Created by subhayu_roy on 4/1/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[255];

    if (argc <3)
    {
        fprintf(stderr,"usage %s hostname port\n",argv[0]);
        exit(1);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0)
        error("Opening socket\n");

    server = gethostbyname(argv[1]);
    if(server == NULL)
        fprintf(stderr,"Error, no such host\n");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)&server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);

    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
        error("Connection failed\n");

    int dataAtrec[10];
    int test[10];
    int p,p1,p2,p4;

    printf("The data received is :\n");
    for (int i = 0; i < 7; ++i) {
        n= read(sockfd,&dataAtrec[i], sizeof(int));
        if (n<0)
            error("Error on receiving data.\n");
        printf("%d",&dataAtrec[i]);
    }

    printf("Enter the data to be tested :\n");
    for (int i=0;i<7;i++)
        scanf("%d",&test[i]);

    p1 = test[6]^test[4]^test[2]^test[0];
    p2 = test[5]^test[4]^test[1]^test[0];
    p4 = test[3]^test[1]^test[2]^test[0];
    p = (4*p4) + (2*p2) + (1*p1);

    if (p == 0)
        printf("No error.\n");
    else
    {
        printf("The error is at position = %d\n",p);
        printf("The correct data is :\n");
        if (test[7-p] == 0)
            test[7-p]=1;
        else
            test[7-p] = 0;
        for (int i=0;i<7;i++)
            printf("%d",test[i]);
    }

    close(sockfd);
    return 0;
}