#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
	if(argc <= 2)
		return 1;
	const char *ip = argv[1];
	int port = atoi(argv[2]);

	struct sockaddr_in address = {0};
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port = htons(port);

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	assert(sock != -1);

	int rcvbuf = atoi(argv[3]);
	int len = sizeof(rcvbuf);
	//先设置缓冲区大小，再读取
	setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &rcvbuf, sizeof(rcvbuf));
	getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &rcvbuf, (socklen_t *)&len);
	printf("the tcp receive buffer size after setting is %d\n", rcvbuf);

	int ret = bind(sock, (struct sockaddr *)&address, sizeof(address));
	assert(ret != -1);

	ret = listen(sock, 5);
	assert(ret != -1);

	struct sockaddr_in client;
	socklen_t client_addrlength = sizeof(client);
	int connfd = accept(sock, (struct sockaddr *)&client, &client_addrlength);
	if(connfd < 0){
		printf("errno: %d\n", errno);
	}
	else{
		char buffer[BUFFER_SIZE];
		memset(buffer, 0, BUFFER_SIZE);
		while(recv(connfd, buffer, BUFFER_SIZE-1, 0) > 0);
		close(connfd);
	}
	close(sock);

	return 0;
}
