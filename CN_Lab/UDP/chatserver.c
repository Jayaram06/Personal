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
	struct sockaddr_in server_addr,client_addr;
	char buffer[MAX_BUFFER_SIZE] = { 0 };
	
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("\n Socket creation failed \n");
		exit(EXIT_FAILURE);
	}
	
	memset(&server_addr,0,sizeof(server_addr));
	memset(&client_addr,0,sizeof(client_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	if (bind(sockfd, (const struct sockaddr*)&server_addr,sizeof(server_addr))< 0) {
		perror("\nBind Failed \n");
		exit(EXIT_FAILURE);
	}

	printf("Server is listening on port %d ...\n",PORT);
	while(1){
		memset(buffer,0,sizeof(buffer));
		socklen_t client_len = sizeof(client_addr);
		int n = recvfrom(sockfd,(char*)buffer,MAX_BUFFER_SIZE,MSG_WAITALL,(struct sockaddr*)&client_addr,&client_len);
		buffer[n] = '\0';
		printf("Client : %s",buffer);
		printf("Server : ");
		fgets(buffer,MAX_BUFFER_SIZE,stdin);
		sendto(sockfd,(const char*)buffer,strlen(buffer),MSG_CONFIRM,(const struct sockaddr*)&client_addr,client_len);
	}
	
	close(sockfd);
	return 0;
}

