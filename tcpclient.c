#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>

#define BUFSIZE 100
#define SADDR struct sockaddr
#define SIZE sizeof(struct sockaddr_in)

int main(int argc, char *argv[]) {
    int fd;
    int nread;
    char *buf;
    struct sockaddr_in servaddr;
    size_t bufsize = 0;
    uint16_t port = 0;
    
    if (argc < 4) {
        printf("Too few arguments \n");
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
    buf = malloc(bufsize * sizeof(char));
    
    struct addrinfo hint;
    struct addrinfo *ainf = NULL;
    memset(&hint, 0, sizeof(struct addrinfo));
    hint.ai_family = AF_INET;
    hint.ai_protocol = 0;
    hint.ai_flags = AI_CANONNAME;
    getaddrinfo(argv[1], argv[2], NULL, &ainf);
    
    
    //if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    if ((fd = socket(ainf->ai_family, ainf->ai_socktype, ainf->ai_protocol)) < 0) {
        perror("socket creating");
        exit(1);
    }

    memset(&servaddr, 0, SIZE);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    
    /*if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        perror("bad address");
        exit(1);
    }*/

    servaddr.sin_port = htons(atoi(argv[2]));

    //if (connect(fd, (SADDR *)&servaddr, SIZE) < 0) {
    if (connect(fd, (struct sockaddr *)(ainf->ai_addr), ainf->ai_addrlen) < 0) {
        perror("connect");
        exit(1);
    }

    write(1, "Input message to send\n", 22);
    while ((nread = read(0, buf, bufsize)) > 0) {
        if (write(fd, buf, nread) < 0) {
            perror("write");
            exit(1);
        }
    }

    close(fd);
    free(buf);
    exit(0);
}
