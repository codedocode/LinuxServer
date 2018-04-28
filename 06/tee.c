#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	if(argc != 2)
		return 1;
	int fileid = open(argv[1], O_CREAT|O_WRONLY|O_TRUNC, 0666);
	assert(fileid > 0);

	int pipefd_stdout[2];
	int ret = pipe(pipefd_stdout);
	assert(ret != -1);
	int pipefd_file[2];
	ret = pipe(pipefd_file);
	assert(ret != -1);

	//ret = splice(STDIN_FIFONO, NULL, pipefd_stdout[1], NULL, 32678, 5);
	ret = splice(0, NULL, pipefd_stdout[1], NULL, 32678, 5);
	assert(ret != -1);
	ret = tee(pipefd_stdout[0], pipefd_file[1], 32768, 2);
	assert(ret != -1);
	ret = splice(pipefd_file[0], NULL, fileid, NULL, 32768, 5);
	assert(ret != -1);
	//ret = splice(pipefd_stdout[0], NULL, STDOUT_FIFONO, NULL, 32768, 5);
	ret = splice(pipefd_stdout[0], NULL, 1, NULL, 32768, 5);
	assert(ret != -1);
	close(fileid);
	close(pipefd_stdout[0]);
	close(pipefd_stdout[1]);
	close(pipefd_file[0]);
	close(pipefd_file[1]);

	return 0;
}
