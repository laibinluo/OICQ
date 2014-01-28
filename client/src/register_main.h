
#ifndef _REGISTER_MAIN_H_
#define _REGINSTER_MAIN_H_

#include"mynet.h"

int register_connnet(struct sockaddr_in *servaddr,
 struct hostent*server,int *sock);

int register_input(clientInfo *c_info);

void register_main();

int register_is(int sock,clientInfo *c_info);

void  print_register_head();

#endif

