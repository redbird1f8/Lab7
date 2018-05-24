#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SERV_PORT 20001
#define BUFSIZE 1024
#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)

int main(int argc, char *argv[]) {
    int sockfd, n;
    char *mesg, ipadr[16];
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
    size_t bufsize = 0;
    uint16_t port = 0;

    if (argc < 3) {
        printf("usage: client <port> <buffer size>\n");
        exit(1);
    }
    
    if(atoi(argv[1]) <= 0){
        printf("Invalid port number.\n");
        exit(1);
    }
    port = atoi(argv[1]);
    
    if(atoi(argv[2]) <= 0){
        printf("Invalid buffer size.\n");
        exit(1);
    }
    bufsize = atoi(argv[2]);
    mesg = malloc(bufsize * sizeof(char));

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket problem");
        exit(1);
    }

    memset(&servaddr, 0, SLEN);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if (bind(sockfd, (SADDR *)&servaddr, SLEN) < 0) {
        perror("bind problem");
        exit(1);
    }
    printf("SERVER starts...\n");

    while (1) {
        unsigned int len = SLEN;

        if ((n = recvfrom(sockfd, mesg, bufsize, 0, (SADDR *)&cliaddr, &len)) < 0) {
            perror("recvfrom");
            exit(1);
        }
        mesg[n] = 0;

        printf("REQUEST %s      FROM %s : %d\n", mesg,
                inet_ntop(AF_INET, (void *)&cliaddr.sin_addr.s_addr, ipadr, 16),
                ntohs(cliaddr.sin_port));

        if (sendto(sockfd, mesg, n, 0, (SADDR *)&cliaddr, len) < 0) {
            perror("sendto");
            exit(1);
        }
    }
    free(mesg);
}
