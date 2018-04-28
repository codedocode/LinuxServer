#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
int buffer_has_item = 0;	//缓冲区计数值
pthread_mutex_t mutex;
int running = 1;
void *producer_f(void *arg)	//生产者
{
	while(running){
		pthread_mutex_lock(&mutex);	//进入互斥区
		buffer_has_item++;
		printf("producer; total: %d\n", buffer_has_item);
		usleep(1);
		pthread_mutex_unlock(&mutex);	//离开互斥区
	}
}

void *consumer_f(void *arg)	//消费者
{
	while(running){
		pthread_mutex_lock(&mutex);
		buffer_has_item--;
		printf("consumer; total: %d\n", buffer_has_item);
		usleep(1);
		pthread_mutex_unlock(&mutex);
	}
}
int main()
{
	pthread_t consumer_t;	//消费者线程参数
	pthread_t producer_t;	//生产者线程参数
	pthread_mutex_init(&mutex, NULL);	//初始化互斥
	pthread_create(&producer_t, NULL, (void *)producer_f, NULL);	//建立生产者线程
	pthread_create(&consumer_t, NULL, (void *)consumer_f, NULL);	//建立消费者线程
	usleep(100);
	running = 0;
	pthread_join(consumer_t, NULL);	//等待消费者线程退出
	pthread_join(producer_t, NULL);	//等待生产者线程退出

	return 0;
}
