#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if(argc <= 2){
		printf("usage: %s ip_address port_number backlog\n", basename(argv[0]));//basename()是取出路径/文件的文件名部分，dirname()和其对应，取出路径部分
		return 1;
	}
	const char *ip = argv[1];
	int port = atoi(argv[2]);

	struct sockaddr_in address = {0};
	address.sin_family = AF_INET;	//设置为IPv4
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port = htons(port);	//要用网络字节序表示

	int sock = socket(PF_INET, SOCK_STREAM, 0);
	assert(sock >= 0);

	int ret = bind(sock, (struct sockaddr *)&address, sizeof(address));	//命名socket,将address强转为sockaddr类型，分配给sock
	assert(ret != -1);
	ret = listen(sock, 5);	//监听socket，指定监听sock，监听队列长度是5
	assert(ret != -1);

	//暂停20s等待客户端连接和相关操作（掉线或退出完成）
	sleep(20);
	struct sockaddr_in client;
	socklen_t client_addrlength = sizeof(client);
	int connfd = accept(sock, (struct sockaddr *)&client, &client_addrlength);
	if(connfd < 0){
		printf("error is %d\n", errno);
	}
	else{
		//接收连接成功则打印出客户端的IP地址和端口号
		char remote[INET_ADDRSTRLEN];
		printf("connected with ip: %s and port: %d\n", inet_ntop(AF_INET, &client.sin_addr, remote, INET_ADDRSTRLEN), ntohs(client.sin_port));
		close(connfd);
	}
	//关闭socket
	close(sock);

	return 0;
}
