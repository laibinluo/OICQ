
#ifndef LINK_ADDR_H
#define LINK_ADDR_H
#include"mynet.h"
#include "linkinfo.h"

/*typedef struct info_addr_node
{
	char name[20];
	struct sockaddr_in addr;
}InfoAddr;*/
typedef struct addr_node
{
	InfoAddr info;
	int sockfd;
	struct addr_node *next;
}linkaddr;

void add_link_addr(linkaddr **paddr,
				  struct sockaddr_in addr,
				  int sockfd);

int delete_link_addr(linkaddr **paddr,int sockfd);

void free_link_addr(linkaddr **paddr);

void print_link_addr(linkaddr *paddr);

int lenght_link_addr(linkaddr *paddr);

linkaddr* select_link_addr(linkaddr *paddr,int sockfd);

linkaddr* select_link_name(linkaddr *paddr,char *name);


#endif
