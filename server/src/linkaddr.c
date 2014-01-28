#include "linkaddr.h"

void add_link_addr(linkaddr **paddr,
				struct sockaddr_in addr,
				 int sockfd)
{
	linkaddr *p=NULL;
	p=(linkaddr*)malloc(sizeof(linkaddr));
	memset(p,0,sizeof(*p));
	(p->info).addr=addr;
	p->sockfd=sockfd;
	
	p->next=*paddr;
	*paddr=p;	
}

int delete_link_addr(linkaddr **paddr,int sockfd)
{
	if(*paddr==NULL)
	{
		return -1;
	}
	linkaddr *p=NULL;
	linkaddr *pre=NULL;
	
	p=*paddr;
	while(p!=NULL)
	{
		if(p->sockfd==sockfd)
		{
			break;
		}
		pre=p;
		p=p->next;		
	}
	
	if(p==NULL)
	{
		return -1;
	}
	if(pre==NULL&&p!=NULL)
	{
		*paddr=(*paddr)->next;
	}
	else
	{
		pre->next=p->next;
	}
	free(p);
	return 0;
}


void free_link_addr(linkaddr **paddr)
{
	linkaddr *p=NULL;
	p=*paddr;
	while(p)
	{
		(*paddr)=(*paddr)->next;
		free(p);
		p=*paddr;	
	}
}

int lenght_link_addr(linkaddr *paddr)
{
	int lenght=0;
	linkaddr *p=paddr;
	while(p)
	{
		if(strlen((p->info).name)!=0)
		{
			lenght++;
		}
		p=p->next;
	}
	return lenght;
}

void print_link_addr(linkaddr *paddr)
{
	linkaddr *p=paddr;
	
	while(p)
	{
		printf("connection from host %s\n,port %d\n,socket %d\nname:%s\r\n",inet_ntoa((p->info).addr.sin_addr),
			   ntohs((p->info).addr.sin_port),
			   p->sockfd,(p->info).name);
		printf("************************\n");
		p=p->next;
	}
}


linkaddr* select_link_name(linkaddr *paddr,char *name)
{
	linkaddr *p=paddr;
	
	while(p)
	{
		if(strcmp((p->info).name,name)==0)
		{
			break;
		}
		p=p->next;
	}
	return p;
}


linkaddr* select_link_addr(linkaddr *paddr,int sockfd)
{
	linkaddr *p=paddr;
	
	while(p)
	{
		if(p->sockfd==sockfd)
		{
			break;
		}
		p=p->next;
	}
	return p;
}



