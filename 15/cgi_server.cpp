#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "processpool.h"

class cgi_conn{
public:
	cgi_conn(){ }
	~cgi_conn(){ }
	void init(int epollfd, int sockfd, const sockaddr_in &client_addr){
		m_epollfd = epollfd;
		m_sockfd = sockfd;
		m_address = client_addr;
		memset(m_buf, 0, BUFFER_SIZE);
		m_read_idx = 0;
	}
	void process(){
		int idx = 0;
		int ret = -1;
		while(true){
			idx = m_read_idx;
			ret = recv(m_sockfd, m_buf + idx, BUFFER_SIZE - 1- idx, 0);
			if(ret < 0){
				if(errno != EAGAIN)
					removefd(m_epollfd, m_sockfd);
				break;
			}
			else if(ret == 0){
				removefd(m_epollfd, m_sockfd);
				break;
			}
			else{
				m_read_idx += ret;
				printf("use content is: %s\n", m_buf);
				for( ; idx < m_read_idx; ++idx){
					if(idx >= 1 && m_buf[idx-1] == '\r' && m_buf[idx] == '\n')
						break;
				}
				if(idx == m_read_idx)
					continue;
				m_buf[idx-1] = '\0';

				char *file_name = m_buf;
				if(access(file_name, F_OK) == -1){
					removefd(m_epollfd, m_sockfd);
					break;
				}
				ret = fork();
				if(ret == -1){
					removefd(m_epollfd, m_sockfd);
					break;
				}
				else if(ret > 0){
					removefd(m_epollfd, m_sockfd);
					break;
				}
				else{
					close(STDOUT_FILENO);
					dup(m_sockfd);
					execl(m_buf, m_buf, 0);
					exit(0);
				}
			}
		}

	}
private:
	static const int BUFFER_SIZE = 1024;
	static int m_epollfd;
	int m_sockfd;
	sockaddr_in m_address;
	char m_buf[BUFFER_SIZE];
	int m_read_idx;
};

int cgi_conn::m_epollfd = -1;

int main(int argc, char *argv[])
{
	if(argc <= 2)
		return 0;
	const char *ip = argv[1];
	int port = atoi(argv[2]);
	int listenfd = socket(PF_INET, SOCK_STREAM, 0);
	assert(listenfd >= 0);
	int ret = 0;
	struct sockaddr_in address = {0};
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port = htons(port);

	ret = bind(listenfd, (struct sockaddr *)&address, sizeof(address));
	assert(ret != -1);
	ret = listen(listenfd, 5);
	assert(ret != -1);
	processpool<cgi_conn> *pool = processpool<cgi_conn>::create(listenfd);
	if(pool){
		pool->run();
		delete pool;
	}
	close(listenfd);
	return 0;

	return 0;
}