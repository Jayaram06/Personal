#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int main()
{
	int sock=0;
	struct sockaddr_in serv_addr;
	char buffer[1024] = { 0 };
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("\n Socket creation failed \n");
		exit(EXIT_FAILURE);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<= 0) {
		perror("\nInvalid address/ Address not supported \n");
		exit(EXIT_FAILURE);
	}

	if ((connect(sock, (struct sockaddr*)&serv_addr,sizeof(serv_addr)))< 0) {
		perror("\nConnection Failed \n");
		exit(EXIT_FAILURE);
	}
	printf("Connected to the server.\n");
	while(1){
		printf("Client : ");
		fgets(buffer,MAX_BUFFER_SIZE,stdin);
		send(sock,buffer,strlen(buffer),0);
		memset(buffer,0,sizeof(buffer));
		int valread = read(sock,buffer,MAX_BUFFER_SIZE);
		if(valread<=0){
		break;
		}
		printf("Server : %s",buffer);
	}

	close(sock);
	return 0;
}

