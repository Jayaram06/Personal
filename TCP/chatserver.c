#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int main()
{
	int server_fd,new_socket;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };
	
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("\n Socket creation failed \n");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(server_fd,(struct sockaddr*)&address,sizeof(address))< 0) {
		printf("\nBind failed\n");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd,3)< 0) {
		perror("\nListen Failed \n");
		exit(EXIT_FAILURE);
	}
	
	printf("Server listening on the port %d...\n",PORT);
	if((new_socket = accept(server_fd,(struct sockaddr*)&address,(socklen_t*)&addrlen))<0){
		perror("Accept failed");
		exit(EXIT_FAILURE);
	}
	while(1){
		memset(buffer,0,sizeof(buffer));
		int valread = read(new_socket,buffer,MAX_BUFFER_SIZE);
		if(valread<=0){
		break;
		}
		printf("Client : %s",buffer);
		printf("Server : ");
		fgets(buffer,MAX_BUFFER_SIZE,stdin);
		send(new_socket,buffer,strlen(buffer),0);
	}

	close(new_socket);
	return 0;
}

