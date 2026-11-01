#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5000
#define BUFFER_SIZE 1024

int main(void)
{
    int server_fd;
    struct sockaddr_in server_addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        perror("TCP socket creation failed");
        return EXIT_FAILURE;
    }

    int reuse = 1;

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
                   &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt");
        close(server_fd);
        return EXIT_FAILURE;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP,
                  &server_addr.sin_addr) != 1) {
        fprintf(stderr, "Invalid server IP address\n");
        close(server_fd);
        return EXIT_FAILURE;
    }

    if (bind(server_fd, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0) {
        perror("TCP bind failed");
        close(server_fd);
        return EXIT_FAILURE;
    }

    if (listen(server_fd, 5) < 0) {
        perror("TCP listen failed");
        close(server_fd);
        return EXIT_FAILURE;
    }

    printf("Dashboard ECU: TCP server started\n");
    printf("Listening on %s:%d\n\n", SERVER_IP, SERVER_PORT);
    printf("Waiting for Vehicle ECU connection...\n");
    fflush(stdout);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        int client_fd = accept(
            server_fd,
            (struct sockaddr *)&client_addr,
            &client_len
        );

        if (client_fd < 0) {
            if (errno == EINTR)
                continue;

            perror("TCP accept failed");
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];

        if (inet_ntop(AF_INET, &client_addr.sin_addr,
                      client_ip, sizeof(client_ip)) == NULL) {
            strcpy(client_ip, "unknown");
        }

        printf("\nVehicle ECU connected from %s:%d\n",
               client_ip, ntohs(client_addr.sin_port));
        fflush(stdout);

        char buffer[BUFFER_SIZE];
        char line[BUFFER_SIZE];
        size_t line_length = 0;

        while (1) {
            ssize_t received = recv(
                client_fd,
                buffer,
                sizeof(buffer),
                0
            );

            if (received < 0) {
                if (errno == EINTR)
                    continue;

                perror("TCP receive failed");
                break;
            }

            if (received == 0) {
                printf("Vehicle ECU disconnected.\n");
                break;
            }

            for (ssize_t i = 0; i < received; i++) {
                if (buffer[i] == '\n') {
                    line[line_length] = '\0';

                    printf("Vehicle Data Received:\n  %s\n\n",
                           line);

                    line_length = 0;
                } else if (line_length < sizeof(line) - 1) {
                    line[line_length++] = buffer[i];
                }
            }

            fflush(stdout);
        }

        close(client_fd);
        printf("Waiting for Vehicle ECU connection...\n");
        fflush(stdout);
    }

    close(server_fd);
    return EXIT_SUCCESS;
}
