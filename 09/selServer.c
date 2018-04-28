#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

int main()
{
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	int result;
	fd_set read_fds, test_fds;
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_address.sin_family = AF_INET;
	inet_pton(PF_INET, "192.168.102.158", &server_address.sin_addr);
	server_address.sin_port = htons(55555);
	int ret = bind(server_sockfd, (struct sockaddr *)&server_address, sizeof(server_address));
	assert(ret != -1);
	ret = listen(server_sockfd, 5);
	assert(ret != -1);

	FD_ZERO(&read_fds);
	FD_SET(server_sockfd, &read_fds);
	while(1){
		char ch;
		int fd;
		int nread;
		test_fds = read_fds;
		printf("server waiting\n"); 
		result = select(FD_SETSIZE, &test_fds, NULL, NULL, NULL);
		assert(result > 0);
		for(fd = 0; fd < FD_SETSIZE; fd++){
			if(FD_ISSET(fd, &test_fds)){
				if(fd == server_sockfd){
					client_len = sizeof(client_address);
					client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
					FD_SET(client_sockfd, &read_fds);
					printf("adding client on fd %d\n", client_sockfd); 
				}
				else{
					ioctl(fd, FIONREAD, nread);
					if(nread == 0){
						close(fd);
						FD_CLR(fd, &read_fds);
						printf("removing client on fd %d\n", fd);
					}
					else{
						read(fd, &ch, 1);
						sleep(5);
						printf("serving client on fd %d\n", fd); 
						ch++;
						write(fd, &ch, 1);
					}
				}
			}

		}
	}

	return 0;
}
