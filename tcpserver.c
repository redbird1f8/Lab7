#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERV_PORT 10050
#define BUFSIZE 100
#define SADDR struct sockaddr

int main(int argc, char *argv[]) {
    const size_t kSize = sizeof(struct sockaddr_in);

    int lfd, cfd;
    int nread;
    char *buf;
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
    buf = malloc(bufsize * sizeof(char));

    if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    memset(&servaddr, 0, kSize);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if (bind(lfd, (SADDR *)&servaddr, kSize) < 0) {
        perror("bind");
        exit(1);
    }

    if (listen(lfd, 5) < 0) {
        perror("listen");
        exit(1);
    }

    while (1) {
        unsigned int clilen = kSize;

        if ((cfd = accept(lfd, (SADDR *)&cliaddr, &clilen)) < 0) {
            perror("accept");
            exit(1);
        }
        printf("connection established\n");

        while ((nread = read(cfd, buf, bufsize)) > 0) {
            //printf("rec bytes: %d\n", nread);
            //printf("%s\n", buf);
            write(1, &buf, nread);
        }

        if (nread == -1) {
            perror("read");
            exit(1);
        }
        close(cfd);
    }
    free(buf);
}
