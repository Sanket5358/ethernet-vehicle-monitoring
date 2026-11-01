
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5000

typedef struct {
    int vehicle_speed;
    int engine_rpm;
    float steering_angle;
} VehicleData;

int main(void)
{
    int sockfd;
    struct sockaddr_in server_addr;
    VehicleData data;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0) {
        perror("UDP socket creation failed");
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

    srand((unsigned int)time(NULL));

    printf("Vehicle ECU: UDP transmission started\n");
    printf("Dashboard IP: %s, Port: %d\n",
           SERVER_IP, SERVER_PORT);

    while (1) {
        data.vehicle_speed = rand() % 121;
        data.engine_rpm = 800 + rand() % 5201;
        data.steering_angle = (float)(rand() % 121 - 60);

        char message[128];

        snprintf(message, sizeof(message),
                 "Speed=%d km/h, RPM=%d, Steering=%.1f degrees",
                 data.vehicle_speed,
                 data.engine_rpm,
                 data.steering_angle);

        ssize_t sent = sendto(
            sockfd,
            message,
            strlen(message) + 1,
            0,
            (struct sockaddr *)&server_addr,
            sizeof(server_addr)
        );

        if (sent < 0) {
            perror("UDP send failed");
        } else {
            printf("Sent: %s\n", message);
        }

        sleep(1);
    }

    close(sockfd);
    return EXIT_SUCCESS;
}

