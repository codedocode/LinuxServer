#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>

pthread_mutex_t mutex;	//互斥锁

void *another(void *arg)
{
	printf("int child thread, lock the mutex\n");
	pthread_mutex_lock(&mutex);
	sleep(5);
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

int main()
{
	pthread_mutex_init(&mutex, NULL);
	pthread_t id;
	pthread_create(&id, NULL, another, NULL);
	sleep(1);
	int pid = fork();
	if(pid < 0){
		pthread_join(id,NULL);
		pthread_mutex_destroy(&mutex);
		return 1;
	}
	else if(pid == 0){
		printf("child want to get the lock\n");
		pthread_mutex_lock(&mutex);
		printf("get the mutex\n");
		pthread_mutex_unlock(&mutex);
		exit(0);
	}
	else{
		wait(NULL);
	}
	pthread_join(id, NULL);
	pthread_mutex_destroy(&mutex);

	return 0;
}
