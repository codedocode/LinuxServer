#include <stdio.h>
#include <signal.h>
typedef void (*sighandler_t)(int);	//定义函数指针
static void sig_handle(int signo)
{
	if(SIGSTOP == signo)
	{
		printf("receive SIGSTOP\n");
	}
	else if(SIGKILL == signo)
	{
		printf("receive SIGKILL\n");
	}
	else
	{
		printf("receive %d\n", signo);
	}
}

int main()
{
	sighandler_t ret;
	ret = signal(SIGSTOP, sig_handle);
	printf("%d\n" ,ret);
	if(SIG_ERR == ret){
		printf("process wrong");
		return -1;
	}
	for(;;) ;

	return 0;
}
