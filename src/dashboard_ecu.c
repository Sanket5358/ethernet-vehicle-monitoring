
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define LISTEN_PORT 5000
#define BUFFER_SIZE 1024

int main(void)
{
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0) {
        perror("UDP socket creation failed");
        return EXIT_FAILURE;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server_addr.sin_port = htons(LISTEN_PORT);

    if (bind(sockfd, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0) {
        perror("UDP bind failed");
        close(sockfd);
        return EXIT_FAILURE;
    }

    printf("Dashboard ECU: UDP receiver started\n");
    printf("Listening on 127.0.0.1:%d\n\n", LISTEN_PORT);

    while (1) {
        ssize_t received = recvfrom(
            sockfd,
            buffer,
            sizeof(buffer) - 1,
            0,
            (struct sockaddr *)&client_addr,
            &client_len
        );

        if (received < 0) {
            perror("UDP receive failed");
            continue;
        }

        buffer[received] = '\0';

        printf("Vehicle Data Received:\n");
        printf("  %s\n\n", buffer);
        fflush(stdout);
    }

    close(sockfd);
    return EXIT_SUCCESS;
}

