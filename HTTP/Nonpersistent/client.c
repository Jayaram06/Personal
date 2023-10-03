#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void send_request(int socket, const char *request) {
    send(socket, request, strlen(request), 0);

    char response[1024];
    memset(response, 0, sizeof(response));
    recv(socket, response, sizeof(response), 0);
    printf("Received response:\n%s\n", response);
}

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    struct hostent *server;

 
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Error in socket");
        exit(EXIT_FAILURE);
    }

    server = gethostbyname("127.0.0.1");
    if (server == NULL) {
        fprintf(stderr, "Error, no such host\n");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_port = htons(8080);
   
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in connect");
        exit(EXIT_FAILURE);
    }


    const char *request1 = "GET /page1 HTTP/1.1\r\nHost: localhost\r\n\r\n";
    printf("Sending request 1:\n%s\n", request1);
    send_request(client_socket, request1);

    close(client_socket);

   
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Error in socket");
        exit(EXIT_FAILURE);
    }

 
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in connect");
        exit(EXIT_FAILURE);
    }

    const char *request2 = "GET /page2 HTTP/1.1\r\nHost: localhost\r\n\r\n";
    printf("Sending request 2:\n%s\n", request2);
    send_request(client_socket, request2);

    close(client_socket);
    return 0;
}

