#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAX_BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[MAX_BUFFER_SIZE];

    // Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error creating socket");
        exit(1);
    }

    printf("Server socket created successfully.\n");

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);  // Port for the server
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in binding");
        exit(1);
    }

    printf("Binding successful.\n");

    // Start listening for incoming connections
    if (listen(server_socket, 10) == 0) {
        printf("Listening...\n");
    } else {
        perror("Error in listening");
        exit(1);
    }

    while (1) {
        // Accept a client connection
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("Error accepting client connection");
            continue;
        }

        printf("Client connected from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Receive the hostname from the client
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received < 0) {
            perror("Error receiving data from client");
            close(client_socket);
            continue;
        }

        buffer[bytes_received] = '\0'; // Null-terminate the received data
        printf("Received hostname from client: %s\n", buffer);

        // Simulate DNS resolution (replace with actual DNS resolution logic)
        const char* resolved_ip = "192.168.1.1"; // Simulated IP address

        // Send the resolved IP address back to the client
        send(client_socket, resolved_ip, strlen(resolved_ip), 0);
        printf("Sent resolved IP address to client: %s\n", resolved_ip);

        close(client_socket);
    }

    close(server_socket);

    return 0;
}
  
