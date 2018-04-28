#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define BUF_SIZE 1024
int main(int argc, char *argv[])
{
	if(argc <= 2){
		return 1;
	}
	const char *ip = argv[1];
	int port = atoi(argv[2]);
	struct sockaddr_in address = {0};
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port = htons(port);
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	assert(sock >= 0);
	int ret = bind(sock, (struct sockaddr *)&address, sizeof(address));
	assert(ret != -1);
	ret = listen(sock, 5);
	assert(ret != -1);
	
	struct sockaddr_in client;
	socklen_t client_addrlength = sizeof(client);
	int connfd = accept(sock, (struct sockaddr *)&client, &client_addrlength);
	if(connfd < 0){
		printf("error is: %d\n", errno);
	}
	else{
		char buffer[BUF_SIZE] = {0};
		memset(buffer, 0, BUF_SIZE);
		ret = recv(connfd, buffer, BUF_SIZE-1, 0);
		printf("got %d bytes of normal data '%s'\n", ret, buffer);

		memset(buffer, 0, BUF_SIZE);
		ret = recv(connfd, buffer, BUF_SIZE-1, MSG_OOB);
		printf("got %d bytes of oob data '%s'\n", ret, buffer);

		memset(buffer, 0, BUF_SIZE);
		ret = recv(connfd, buffer, BUF_SIZE-1, 0);
		printf("got %d bytes of normal data '%s'\n", ret, buffer);

	}
	close(sock);

	return 0;
}
