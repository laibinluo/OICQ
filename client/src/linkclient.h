
#ifndef LINK_CLIENT_H
#define LINK_CLIENT_H

#include "mynet.h"

typedef struct client_login_info_node
{
	InfoAddr info;
	int number;
	struct client_login_info_node *next;
}linkaddr;

int print_link_client(linkaddr *paddr,clientInfo info);

void free_link_client(linkaddr **paddr);

linkaddr *select_link_client(linkaddr *paddr,int number);

void add_link_client(linkaddr **paddr,InfoAddr *info);

int delete_link_client(linkaddr **paddr,InfoAddr *info);

linkaddr*select_link_client_info(linkaddr* paddr,clientInfo info);

#endif




