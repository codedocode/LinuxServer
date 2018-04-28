#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
int main()
{
	int result = -1;	//创建管道结果
	int fd[2];	//文件描述符
	pid_t pid;	//PID值
	int nbytes;
	char string[] = "123456789";
	char readbuffer[80];
	//文件描述符1用来写，文件描述符0用来读
	int *write_fd = &fd[1];
	int *read_fd = &fd[0];
	result = pipe(fd);
	if(-1 == result){
		printf("管道建立失败\n");
		return -1;
	}
	pid = fork();	//分叉程序
	if(-1 == pid){
		printf("fork进程失败\n");
		return -1;
	}
	if(0 == pid){	//子进程关闭读端
		close(*read_fd);
		result = write(*write_fd, string, strlen(string));
	}
	else{	//父进程关闭写端
		close(*write_fd);
		nbytes = read(*read_fd, readbuffer, sizeof(readbuffer));
		printf("receice %d , content:%.*s\n", nbytes, nbytes, readbuffer);
	}
	
	return 0;
}
