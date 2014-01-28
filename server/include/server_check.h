

#ifndef SERVER_CHECK_H
#define SERVER_CHECK_H

#include"mynet.h"
#include "linkinfo.h"
#include "linkaddr.h"

typedef struct time_sock
{
	time_t t_begin;
	int sock;
	struct time_sock *next;
}t_client;

void is_register(int fd,linkinfo **pinfo,clientInfo *info);

linkinfo* is_login(int fd,linkinfo *pinfo,clientInfo *info,linkaddr *paddr);

linkinfo* check_info(int fd,char flag,linkinfo **pinfo,clientInfo *info,linkaddr *paddr);

char read_pack_head(int fd,pack_head *ph);

void write_pack_head(int fd,pack_head *ph);

void is_error(int fd);

void read_client_info(int fd,clientInfo *info);

int lenght_client_info(linkaddr *paddr,clientInfo *info,int sockfd);

void write_link_addr(linkaddr *paddr,int sockfd);
					 
void write_new_addr_login(linkaddr *paddr,
						  char *name,char type);
						  
void  add_time_link(int sock,t_client **ptt);

t_client* selete_time_link(int sock,t_client*pt);

int delete_time_link(int sock,t_client **pt);

#endif
