
#ifndef LOGIN_MAIN_H
#define LOGIN_MAIN_H

#include "mynet.h"
#include "linkclient.h"

typedef struct read_info_node
{
	char name[10];
	char message[256];
	struct read_info_node *next;
}linkmessage;

void  print_login_head();

int login_connnet(struct sockaddr_in *servaddr,
				  struct hostent*server,int*sock);

void login_main();

void logout_client(int sock);

int login_input(clientInfo *c_info);

int login_is(int sock,clientInfo *c_info,pack_head *ph);

void read_link_addr(linkaddr **paddr,
					int sockfd,pack_head *ph);
					
int read_new_client(linkaddr **paddr,int sockfd);


void thread_login(int *sock);

void print_send_infomation(linkaddr *paddr,
						   clientInfo info,int udp_sock);

void print_read_infomation(linkaddr *paddr,int sock);

int manage_client_connet(linkaddr *pclient);

void thread_read(int *sock);
#endif



