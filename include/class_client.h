#ifndef __Client
#define __Client
#include<stdio.h>
#include<stdlib.h>
#include<netdb.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include <arpa/inet.h>
#include"unistd.h"
#include<iostream>
using std::cout;
using std::endl;
using std::string;
class Client{
	public:
	Client(string str,const char *port);
	Client();
	~Client(){
	close(sockfd);
	}
	static string recv(void);
	static int send(string str);
	int socket_status();//返回socket状态，0为连接超时，小于0为连接错误
	void sclose(void);
	int fd(void);
	void restart(string argv,int ch=0);
	private:
	void freeaddr(void);
	char ipaddr[INET6_ADDRSTRLEN];
	struct sockaddr_in *dest_addr;
	struct sockaddr_in6 *dest_addrv6;
	struct addrinfo hints,*res=NULL;
	string buf;
	static int sockfd;
	static unsigned int buf_max;
	int client_ipv4();
	int client_ipv6();
};

#endif