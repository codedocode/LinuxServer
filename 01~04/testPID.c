#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	pid_t pid, ppid;	//当前进程号和父进程号
	//获得进程号和父进程号
	pid = getpid();
	ppid = getppid();

	printf("process:%d;parent process:%d\n", pid, ppid);

	return 0;
}
