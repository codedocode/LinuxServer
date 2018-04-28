#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>

static char msg[] = "hello, shared memory\n";

typedef int sem_t;

union semun{					//信号量操作的联合结构
	int			val;			//整型变量
	struct		semid_ds *buf;	//semid_ds结构指针
	unsigned	*array;			//数组类型
} arg;							//定义一个全局变量
/*
struct sembuf{
	ushort sem_num;		//信号量的编号
	short sem_op;		//信号量的操作
	short sem_flg;		//信号量的操作标志
}
*/
int Sem_P(sem_t semid)
{
	struct sembuf sops = {0, +1, IPC_NOWAIT};

	return (semop(semid, &sops, 1));
}

int Sem_V(sem_t semid)
{
	struct sembuf sops = {0, -1, IPC_NOWAIT};

	return (semop(semid, &sops, 1));
}

//建立信号量，魔数key和信号量的初始值value
sem_t CreateSem(key_t key, int value)
{
	union semun sem;	//信号量结构变量
	sem_t semid;		//信号量id
	sem.val = value;	//设置初始值
	
	semid = semget(key, 0, IPC_CREAT | 0666);	//获得信号量的ID
	if(-1 == semid){
		printf("create semaphore failed\n");
		return -1;
	}
	semctl(semid, 0, SETVAL, sem);	//发送命令，建立value个初始值的信号量

	return semid;	//返回建立的信号量
}

int DestroySem(sem_t semid)	//信号量的值
{
	union semun sem;	//信号量操作的结构
	sem.val = 0;		//信号量值的初始化
	semctl(semid, 0, IPC_RMID, sem);	//设置初始值
}

int main(void)
{
	key_t key;
	int semid, shmid;
	char i, *shms, *shmc;
	struct shmid_ds buf;
	int value = 0;
	char buffer[80];
	pid_t p;
	
	key = ftok("./", 'a');	//生成键值
	shmid = shmget(key, 1024, IPC_CREAT | 0604);	//获得共享内存，大小为1024
	semid = CreateSem(key, 0);	//建立信号量

	p = fork();
	if(p > 0){	//父进程
		shms = (char *)shmat(shmid, 0, 0);	//挂接共享内存
		memcpy(shms, msg, strlen(msg) + 1);
		sleep(10);	//等待10秒，另一个进程将数据读出
		Sem_P(semid);	//获得共享内存的信号量
		shmdt(shms);	//摘除共享内存
		DestroySem(semid);	//销毁信号量
	}
	else if(p == 0){	//子进程
		shmc = (char *)shmat(shmid, 0, 0);
		Sem_V(semid);	//减小信号量
		printf("the value in shared memory is %s\n", shmc);	//打印信息
		shmdt(shmc);	//摘除共享内存
	}

	return 0;
}
