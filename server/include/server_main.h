
#ifndef SERVER_MAIN_H
#define SERVER_MAIN_H

#include "mynet.h"
#include "linkaddr.h"
#include "linkinfo.h"
#include "server_check.h"

int server_listen();

int lenght_info(linkaddr *paddr);

void server_main();

/*void write_link_addr(linkaddr *paddr,
					linkinfo *pclient,int sockfd);

void write_new_addr_login(linkaddr *paddr,
				  linkinfo *pclient,char type);*/
//void is_online(int sockfd);
void is_active_client();

#endif


