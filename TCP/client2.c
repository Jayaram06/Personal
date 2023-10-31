// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFFER_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create TCP socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to the echo server.\n");

    while (1) {
        // Read input from the user
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Send the message to the server
        send(client_fd, buffer, strlen(buffer), 0);

        // Receive the echo from the server
        int valread = read(client_fd, buffer, BUFFER_SIZE);
        if (valread > 0) {
            printf("Server's response: %s\n", buffer);
        }
    }

    close(client_fd);

    return 0;
}
