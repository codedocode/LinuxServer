#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define K 1024
#define WRITELEN (128 * K)

int main()
{
	int result = -1;	//创建管道结果
	int fd[2];	//文件描述符
	pid_t pid;	//PID值
	int nbytes;
	//char string[WRITELEN] = "123456789";
	char string[WRITELEN] = {0};
	char readbuffer[10*K];
	//文件描述符1用来写，文件描述符0用来读
	int *write_fd = &fd[1];
	int *read_fd = &fd[0];
	result = pipe(fd);	//建立管道
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
		int write_size = WRITELEN;
		result = 0;
		while(write_size >= 0){
			result = write(*write_fd, string, write_size);
			if(result > 0){
				write_size -= result;
				printf("write %d data, last %d data\n", result, write_size);
			}
			else{
				return 0;
			}
		}
	}
	else{	//父进程关闭写端
		close(*write_fd);
		while(1){
			nbytes = read(*read_fd, readbuffer, sizeof(readbuffer));
			if(nbytes <= 0){
				printf("no more data\n");
				break;
			}
			printf("receice %d , content:%s\n", nbytes, readbuffer);
		}
	}
	
	return 0;
}
