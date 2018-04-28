#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>

#define BUFFER_SIZE 1023
int setnonblocking(int fd)
{
	int old_option = fcntl(fd, F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd, F_SETFL, new_option);
	return old_option;
}

int unblock_connect(const char *ip, int port, int time)
{
	int ret = 0;
	struct sockaddr_in address = {0};
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port = htons(port);

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	int fdopt = setnonblocking(sockfd);
	ret = connect(sockfd, (struct sockaddr *)&address, sizeof(address));
	if(ret == 0){
		printf("connect with server immediately\n");
		fcntl(sockfd, F_SETFL, fdopt);
		return sockfd;
	}
	else if(errno != EINPROGRESS){
		printf("unblock connect not support\n");
		return -1;
	}

	fd_set readfds;
	fd_set writefds;
	struct timeval timeout;
	FD_ZERO(&readfds);
	FD_SET(sockfd, &writefds);
	timeout.tv_sec = time;
	timeout.tv_usec = 0;

	ret = select(sockfd + 1, NULL, &writefds, NULL, &timeout);
	if(ret <= 0){
		printf("connection time out\n");
		close(sockfd);
		return -1;
	}
	if(!FD_ISSET(sockfd, &writefds)){
		printf("no events on sockde found\n");
		close(sockfd);
		return -1;
	}
	int error = 0;
	socklen_t length = sizeof(error);
	if(getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &length) < 0){
		printf("get socket option failed\n");
		close(sockfd);
		return -1;
	}
	if(error != 0){
		printf("error:%d\n", error);
		printf("connection failed after select with the error\n", error);
		close(sockfd);
		return -1;
	}
	printf("connection ready after select with the socket\n", sockfd);
	fcntl(sockfd, F_SETFL, fdopt);
	return sockfd;
}

int main(int argc, char *argv[])
{
	if(argc <= 2)
		return 1;
	const char *ip = argv[1];
	int port = atoi(argv[2]);

	int sockfd = unblock_connect(ip, port, 10);
	if(sockfd < 0)
		return 1;
	close(sockfd);

	return 0;
}
