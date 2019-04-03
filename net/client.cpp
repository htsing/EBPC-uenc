#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>

using namespace std;


#define SER "47.74.228.21"
#define PORT 8888

typedef struct
{
    struct in_addr ip;
    int port;
}IP; //ip与端口

typedef struct sockaddr SA;
typedef struct sockaddr_in SA_IN;

//回射服务
void write_echo(int sockfd)
{
    char buf[1024];
    
    printf("echo.\n");
    while(1)
    {
        bzero(buf,sizeof(buf));
        
        printf(">");
        scanf("%s",buf);
        
        send(sockfd,buf,strlen(buf),0);
        
        recv(sockfd,buf,sizeof(buf)-1,0);
        printf("recv:%s\n",buf);

        buf[strlen(buf)]='\0';
        if(strcmp(buf,"exit") == 0)
            break;
    }
}

int main(int argc,char **argv)
{
    int sockfd,sockfd2;
    SA_IN server,addr,client;
    IP ip;
    char ser_ip[32] ;
    bzero(ser_ip,32);
    socklen_t addrlen=sizeof(SA_IN);
    char s = 'a';
    int flags=1;
    int port = 0;
    int ret = 0;    

    if(argc >= 2)
        strcpy(ser_ip,argv[1]);

    if(argc == 3)
        port = atoi(argv[2]);
    
    printf("ip:%s\n",ser_ip);
    printf("port:%d\n",port);
  
  
   
    sockfd = socket(AF_INET,SOCK_STREAM,0);
   

    //端口重用
    if((setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&flags,sizeof(int))) ==-1)
        perror("setsockopt sockfd");
    
    bzero(&addr,sizeof(SA_IN));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
  
    ret = bind(sockfd,(SA *)&addr,sizeof(SA_IN));
    if(ret < 0)
        perror("bind sockfd");
 
 
    bzero(&server,sizeof(SA_IN));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ser_ip);
    server.sin_port = htons(port);
    
    ret = connect(sockfd,(SA *)&server,sizeof(SA_IN));

    if(ret >= 0)
    {    
        printf("recv...\n");
        recv(sockfd,&ip,sizeof(IP),0);
        printf("recv over\n");
        printf("ip:%s    port:%d\n",inet_ntoa(ip.ip),ip.port);
    }
    
    if(ret < 0)
       perror("connect");  


    sockfd2=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd2 == -1)
        perror("sockfd2");
    
    if((setsockopt(sockfd2,SOL_SOCKET,SO_REUSEADDR,&flags,sizeof(int)))== -1)
        perror("setsockopt sockfd2");

   
    ret = bind(sockfd2,(SA *)&addr,sizeof(SA_IN));
    if(ret < 0)
        perror("bind sockfd");
 
    bzero(&client,sizeof(SA_IN));
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = inet_addr(inet_ntoa(ip.ip));
    client.sin_port=htons(ip.port);
    
    while(1)
    {
        ret = connect(sockfd2,(SA *)&client,sizeof(SA_IN));

        if(ret < 0)
            printf("%s %d %s",inet_ntoa(ip.ip),ip.port,strerror(ret));
        
        if(ret == 0)
        {
            printf("connect %s %d success.",inet_ntoa(ip.ip),ip.port);
            break;
        }

        if(ret > 0)
            printf("ret:%d\n",ret);

    } 

    write_echo(sockfd2);

    while(1)
    {
        sleep(100);    
    }
    
    cout << "try hard!!!" << endl;

    return 0;
}
