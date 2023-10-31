#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    int num;

    // Create TCP socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to a specific IP and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Echo server is listening on port %d...\n", PORT);

    while (1) {
        // Accept a new client connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("Client connected.\n");

        while (1) {
            // Read data from the client
            valread = read(new_socket, &num, sizeof(num));
            if (valread == 0) {
                printf("Client disconnected.\n");
                break;
            } else if (valread < 0) {
                perror("read");
                break;
            }

            // Calculate the square of the number
            int squared_num = num * num;

            // Send the squared number back to the client
            send(new_socket, &squared_num, sizeof(squared_num), 0);
            printf("Sent squared number: %d\n", squared_num);
        }

        close(new_socket);
    }

    return 0;
}

