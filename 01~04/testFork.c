#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	pid_t pid;
	//分叉进程
	pid = fork();
	//判断是否执行成功
	if(-1 == pid){
		printf("进程创建失败\n");
		return -1;
	}
	else if(0 == pid){
		//子进程中
		printf("child; return ID: %d; ID :%d; father: %d\n", pid, getpid(), getppid());
	}
	else{
		//父进程中
		printf("father; return ID: %d; ID :%d; father: %d\n", pid, getpid(), getppid());
	}

	return 0;
}
