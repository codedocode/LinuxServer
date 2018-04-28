#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
//typedef enum{false = 0, true} bool;
static int stop = 0;
//SIGTEM信号的处理函数，触发时结束主程序中的循环
static void handle_term(int sig)
{
	stop = 1;
}

int main(int argc, char *argv[])
{
	signal(SIGTERM, handle_term);//设置一个函数来处理信号

	if(argc < 3){
		printf("usage: %s ip_address port_number backlog\n", basename(argv[0]));//basename()是取出路径/文件的文件名部分，dirname()和其对应，取出路径部分
		return 1;
	}
	const char *ip = argv[1];
	int port = atoi(argv[2]);
	int backlog = atoi(argv[3]);

	//printf("123435\n");
	int sock = socket(PF_INET, SOCK_STREAM, 0);	//创建IP-TCP-默认协议 的socket
	assert(sock >= 0);
	//创建一个ipv4 socket地址
	struct sockaddr_in address = {0};
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port = htons(port);	//要用网络字节序表示
	
	//printf("%d\n", sizeof(address));
	int ret = bind(sock, (struct sockaddr *)&address, sizeof(address));	//命名socket,将address强转为sockaddr类型，分配给sock
	//printf("%d\n", sizeof(address));
	assert(ret != -1);
	ret = listen(sock, backlog);	//监听socket，指定监听sock，监听队列长度是5
	assert(ret != -1);

	//循环等待连接，直到有SIGTERM信号将其中断
	while(!stop){
		sleep(1);
	}
	//关闭socket
	close(sock);

	return 0;
}
