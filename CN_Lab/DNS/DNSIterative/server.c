#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main() {
    int sockfd, new_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[1024];
    char *ip_address = "127.0.0.2"; // Change this to your server's IP address

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error in socket creation");
        exit(1);
    }

    printf("Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr(ip_address);

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in binding");
        exit(1);
    }

    printf("Binding successul.\n");

    if (listen(sockfd, 10) == 0) {
        printf("Listening...\n");
    } else {
        printf("Error in listening.\n");
        exit(1);
    }

    addr_size = sizeof(new_addr);
    new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);

    // DNS lookup
    char host_buffer[256];
    char service_buffer[256];
    struct sockaddr_in peer_addr;
    socklen_t peer_addr_size = sizeof(peer_addr);

    if (getnameinfo((struct sockaddr*)&new_addr, addr_size, host_buffer, sizeof(host_buffer), service_buffer, sizeof(service_buffer), 0) == 0) {
        printf("Client connected from host: %s, port: %s\n", host_buffer, service_buffer);
    } else {
        printf("Unable to resolve client address.\n");
    }

    close(new_sock);
    close(sockfd);

    return 0;
}

