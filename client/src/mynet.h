/*
*	filename   :mynet.h
*	author     :luolaibin
*	create     :2012-4-20
*
*/
#ifndef  _MYNET_H_
#define  _MYNET_H_

#include<sys/types.h>
#include<ctype.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<ctype.h>
#include<errno.h>
#include<sys/time.h>
#include<stdio.h>
#include<strings.h>
#include<sys/select.h>
#include<stdlib.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <time.h>

#define VER_TCP   '1'
#define VER_UDP   '2'
 
#define REGISTER  'R'
#define LOGIN     'I'
#define LOGOUT    'Q'
#define YES       'Y'
#define NO		  'N'
#define CLOSEED    'E'
#define NEW		  'A'
#define DELETE    'D'
#define  ONLINE    'O'
#define  LIFE	'L'

#define  SERVER_IP  "192.168.1.176"
#define  PORT  2012
#define UDP_PROT  2015

#define LISTEN_QUEUE_NUM 5
#define BUFFER_SIZE 256
#define CLIENT_INFO  'C'

#define RET_ERR -1

//pack_head information
typedef struct pack_head_node
{
	unsigned char ver;
	unsigned char type;
	unsigned short len;
}pack_head;


//client information,the information save to file
typedef struct client_info_node
{
	char name[20];
	char password[10];
}clientInfo;

typedef struct info_addr_node
{
	char name[20];
	struct sockaddr_in addr;
}InfoAddr;

/*void  print_head()
{
	printf("\t\t*****************************************\n");
	printf("\t\t*****************************************\n");
	printf("\t\t*************欢迎使用本系统**************\n");
	printf("\t\t*****************************************\n");
	printf("\t\t*****************************************\n");
}
*/

#endif






























