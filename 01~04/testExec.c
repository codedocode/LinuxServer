#include <stdio.h>
#include <unistd.h>

int main()
{
	char *args[] = {"/usr/bin/ls", NULL};
	printf("系统分配的进程号是: %d\n", getpid());
	if(execve("PATH = /usr/bin/ls", args, NULL) < 0)
		printf("创建进程出错\n");
	
	return 0;
}
