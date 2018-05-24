#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SERV_PORT 20001
#define BUFSIZE 1024
#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)

int main(int argc, char **argv) {
    int sockfd, n;
    char *sendline, *recvline;
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
    size_t bufsize = 0;
    uint16_t port = 0;
    
    if (argc < 4) {
        printf("usage: client <IPaddress of server> <port> <buffer size>\n");
        exit(1);
    }
    
    if(atoi(argv[2]) <= 0){
        printf("Invalid port number.\n");
        exit(1);
    }
    port = atoi(argv[2]);
    
    if(atoi(argv[3]) <= 0){
        printf("Invalid buffer size.\n");
        exit(1);
    }
    bufsize = atoi(argv[3]);
    sendline = malloc(bufsize * sizeof(char));
    recvline = malloc(bufsize+1 * sizeof(char));

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0) {
        perror("inet_pton problem");
        exit(1);
    }
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket problem");
        exit(1);
    }

    write(1, "Enter string\n", 13);

    while ((n = read(0, sendline, bufsize)) > 0) {
        if (sendto(sockfd, sendline, n, 0, (SADDR *)&servaddr, SLEN) == -1) {
            perror("sendto problem");
            exit(1);
        }

        if (recvfrom(sockfd, recvline, bufsize, 0, NULL, NULL) == -1) {
            perror("recvfrom problem");
            exit(1);
        }

        printf("REPLY FROM SERVER= %s\n", recvline);
    }
    close(sockfd);
    free(sendline);
    free(recvline);
}
