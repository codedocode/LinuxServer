#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	int ret;
	printf("系统分配的进程号是: %d\n", getpid());
	ret = system("ping www.baidu.com -c 2");
	printf("return : %d\n", ret);

	return 0;
}
