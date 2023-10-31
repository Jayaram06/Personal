#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int main()
{
	int sockfd;
	struct sockaddr_in server_addr;
	char buffer[MAX_BUFFER_SIZE] = { 0 };
	
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("\n Socket creation failed \n");
		exit(EXIT_FAILURE);
	}
	
	memset(&server_addr,0,sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr)<= 0) {
		perror("\nInvalid address/ Address not supported \n");
		exit(EXIT_FAILURE);
	}

	printf("Connected to the server.\n");
	while(1){
		printf("Client : ");
		socklen_t server_len = sizeof(server_addr);
		fgets(buffer,MAX_BUFFER_SIZE,stdin);
		sendto(sockfd,(const char*)buffer,strlen(buffer),MSG_CONFIRM,(const struct sockaddr*)&server_addr,sizeof(server_addr));
		int n = recvfrom(sockfd,(char*)buffer,MAX_BUFFER_SIZE,MSG_WAITALL,(struct sockaddr *)&server_addr,&server_len);
		buffer[n] = '\0';
		printf("Server : %s",buffer);
	}

	close(sockfd);
	return 0;
}

