
#ifndef LINKINFO_H
#define LINKINFO_H

#include "mynet.h"

#define PATH_CLIENT "../data/c_info"

typedef struct link_client_node
{
	clientInfo info;
	struct link_client_node *next;
}linkinfo;

void creat_link_info(linkinfo **pinfo);

void print_link_info(const linkinfo *pinfo);

void free_link_info(linkinfo **pinfo);

void save_client_info(linkinfo* pinfo);

void read_link_info(linkinfo **pinfo);

int add_link_info(linkinfo **pinfo,clientInfo info);

linkinfo *select_link_info(linkinfo *pinfo,clientInfo info);

#endif 







