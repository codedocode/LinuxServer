/* 
 * 线程实例
 */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

static int run = 1;		//运行状态参数
static int retvalue;	//线程返回值
void *start_routine(void *arg)	//线程处理函数
{
	int *running = arg;	//获取运行状态指针
	printf("child thread init finished, argment is %d\n", *running);
	while(*running){
		printf("child thread is running\n");
		usleep(1);
	}
	printf("child thread drop out");

	retvalue = 8;	//设置退出值
	pthread_exit((void *)&retvalue);	//线程退出并设置退出值
}

int main()
{
	pthread_t pt;
	int ret = -1;
	int times = 3;
	int i = 0;
	int *ret_join = NULL;
	ret = pthread_create(&pt, NULL, (void *)start_routine, &run);
	if(ret != 0){
		printf("create thread failed\n");
		return 1;
	}
	usleep(1);
	for(; i < times; i++){
		printf("main thread print\n");
		usleep(1);
	}
	run = 0;
	pthread_join(pt, (void *)&ret_join);	//等待线程退出
	printf("thread return value %d\n", *ret_join);

	return 0;
}
