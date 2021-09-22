#include"class_client.h"
#include"sys/ioctl.h"
int Client::sockfd=0;
unsigned int Client::buf_max=3333;//最大接受数据长度
int socket_timeout(int,int);
Client::Client(string str,const char *port){
	bzero(&hints,sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	//hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    int err = getaddrinfo(str.c_str(),port,&hints,&res);
	if(err)
	{
		printf("error %d: %s\n",err,gai_strerror(err));
		exit(0);
	}
	
	if(res->ai_family==AF_INET6){
	dest_addrv6=(struct sockaddr_in6 *)res->ai_addr;
	inet_ntop(AF_INET6,&dest_addrv6->sin6_addr,ipaddr,INET6_ADDRSTRLEN);
	//cout<<"正在连接到ipv6服务器: "<<ipaddr<<endl;
	client_ipv6();
	}
	else{
	dest_addr=(struct sockaddr_in *)res->ai_addr;
	inet_ntop(AF_INET,&dest_addr->sin_addr,ipaddr,INET_ADDRSTRLEN);
	//cout<<"正在连接到ipv4服务器: "<<ipaddr<<endl;
	client_ipv4();
	}
}

Client::Client():ipaddr(""),buf(""){
}

int Client::client_ipv4(){
	sockfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);/*建立socket*/
	if(sockfd==-1){
		printf("socket failed:%d",errno);
		return -1;
	}
	int ul=1;
	int timeout=5000;
	ioctl(sockfd,FIONBIO,&ul);//设置非阻塞
	
	if(connect(sockfd,res->ai_addr,res->ai_addrlen)==1){//连接方法，传入句柄，目标地址和大小
	
        // 设置为阻塞
       
        ul = 0;
        ioctl(sockfd, FIONBIO, &ul);
        setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(int));
        return sockfd;
	}
	if(socket_timeout(sockfd,3)<=0){
	cout<<"连接超时！"<<endl;
	close(sockfd);
	exit(0);
	}
	ul = 0;
    ioctl(sockfd, FIONBIO, &ul);//设置为阻塞
	setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(int));
	freeaddr();
	return sockfd;
}

int Client::client_ipv6(){
	sockfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);/*建立socket*/
	if(sockfd==-1){
		printf("socket failed:%d",errno);
		return -1;
	}
	
 	int ul=1;
 	int timeout=5000;
	ioctl(sockfd,FIONBIO,&ul);//设置非阻塞
	if(connect(sockfd,res->ai_addr,res->ai_addrlen)==1){//连接方法，传入句柄，目标地址和大小 
		// 设置为阻塞
        ul = 0;
        ioctl(sockfd, FIONBIO, &ul);
        setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(int));
        return sockfd;
	}
	if(socket_timeout(sockfd,3)<=0){
	cout<<"连接超时！"<<endl;
	close(sockfd);
	exit(0);
	}
	ul = 0;
    ioctl(sockfd, FIONBIO, &ul);//设置为阻塞
	setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(int));
	freeaddr();
	return sockfd;
}

int socket_timeout(int sockfd, int s)
{
    int re = 0;
    fd_set set;
    struct timeval tm;

    int len;
    int error = -1;

    tm.tv_sec = s;
    tm.tv_usec = 0;
    FD_ZERO(&set);
    FD_SET(sockfd, &set);

    re = select(sockfd + 1, NULL, &set, NULL, &tm);
    if(re > 0){//select超时返回0，发生错误返回-1
        len = sizeof(int);
        // 获取socket状态
        getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
        if(error == 0){
            re = 1;
        }else{
            re = -3;
        }
    }else{
    //cout<<"超时"<<endl;
        re = 0;
    }

    return re;
}

int Client::socket_status(){
return sockfd;
}

string Client::recv(){
	char buf[buf_max];
	int len=::recv(sockfd,buf,buf_max,0);
	if(len){
	buf[len]='\0';
	return buf;
	}
	else{
		close(sockfd);
		cout<<"远端服务器以关闭，退出程序！"<<endl;
	}
	return "\0";

}

int Client::send(string str){
	return ::send(sockfd,str.c_str(),str.size(),0);
}
	
void Client::sclose(void){
	close(sockfd);
}

int Client::fd(void){
return sockfd;
}
void Client::freeaddr(void){
freeaddrinfo(res);
}
	
void Client::restart(string argv,int ch){
char *str[]={(char *)argv.c_str(),(char *)"4",(char *)0};//此为execv argv参数
sclose();
switch(ch){
case 0:
{
char *str[]={(char *)argv.c_str(),(char *)0};
execv("/proc/self/exe",str);//exe为自身运行程序的路径，str为参数，
break;
}
case 4:
{
//char *str[]={(char *)argv.c_str(),"4",(char *)0};
execv("/proc/self/exe",str);
break;
}
case 6:
{
//char *str[]={(char *)argv.c_str(),"6",(char *)0};
str[1]=(char *)"6";
execv("/proc/self/exe",str);
break;
}
}
exit(0);

}