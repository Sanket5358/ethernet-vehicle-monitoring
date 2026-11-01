#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5000
#define BUFFER_SIZE 256

int main(void)
{
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("TCP socket creation failed");
        return EXIT_FAILURE;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP,
                  &server_addr.sin_addr) != 1) {
        fprintf(stderr, "Invalid server IP address\n");
        close(sockfd);
        return EXIT_FAILURE;
    }

    printf("Vehicle ECU: Connecting to Dashboard ECU...\n");

    if (connect(sockfd, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0) {
        perror("TCP connection failed");
        close(sockfd);
        return EXIT_FAILURE;
    }

    printf("TCP connection established successfully!\n");
    printf("Sending vehicle data every second.\n");
    printf("Press Ctrl+C to stop.\n\n");

    srand((unsigned int)time(NULL));

    while (1) {
        int speed = rand() % 121;
        int rpm = 800 + rand() % 5201;
        float steering = (float)(rand() % 121 - 60);

        int length = snprintf(
            buffer,
            sizeof(buffer),
            "Speed=%d km/h, RPM=%d, Steering=%.1f degrees\n",
            speed,
            rpm,
            steering
        );

        if (length < 0 || length >= (int)sizeof(buffer)) {
            fprintf(stderr, "Failed to format vehicle data\n");
            break;
        }

        size_t total_sent = 0;

        while (total_sent < (size_t)length) {
            ssize_t sent = send(
                sockfd,
                buffer + total_sent,
                (size_t)length - total_sent,
                0
            );

            if (sent < 0) {
                if (errno == EINTR)
                    continue;

                perror("TCP send failed");
                close(sockfd);
                return EXIT_FAILURE;
            }

            if (sent == 0) {
                fprintf(stderr, "TCP connection closed\n");
                close(sockfd);
                return EXIT_FAILURE;
            }

            total_sent += (size_t)sent;
        }

        printf("Transmitted: %s", buffer);
        sleep(1);
    }

    close(sockfd);
    return EXIT_SUCCESS;
}
