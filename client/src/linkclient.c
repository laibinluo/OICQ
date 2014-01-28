#include "linkclient.h"

int print_link_client(linkaddr *paddr,clientInfo info)
{	
	linkaddr *p=paddr;
	int i=1;	
	while(p)
	{
		
		if(strcmp((p->info).name,info.name)==0)
		{
			p->number=-1;
			p=p->next;
			continue;
		}
		p->number=i;
		printf("\t\t\t%d.%s\n",
			   p->number,(p->info).name);
		//printf("the number host %s\n,port %d\n",
			   //inet_ntoa((p->info).addr.sin_addr),ntohs((p->info).addr.sin_port));
		i++;
		p=p->next;
	}
	return i--;
}

void free_link_client(linkaddr **paddr)
{
	linkaddr *p=*paddr;
	
	while(*paddr)
	{
		*paddr=(*paddr)->next;
		free(p);
		p=*paddr;
	}
}

linkaddr *select_link_client_info(linkaddr *paddr,clientInfo info)
{
	linkaddr *p=paddr;
	while(p)
	{
		if(strcmp((p->info).name,info.name)==0)
		{
			break;
		}
		p=p->next;
	}
	return p;
}

linkaddr *select_link_client(linkaddr *paddr,int number)
{
	linkaddr *p=paddr;
	while(p)
	{
		if(p->number==number)
		{
			break;
		}
		p=p->next;
	}
	return p;
}

void add_link_client(linkaddr **paddr,InfoAddr *info)
{
	linkaddr *p=NULL;
	
	p=(linkaddr*)malloc(sizeof(linkaddr));
	p->info=*info;
	p->number=0;
	
	p->next=*paddr;
	(*paddr)=p;
}

int delete_link_client(linkaddr **paddr,InfoAddr *info)
{
	linkaddr *p=NULL;
	linkaddr *pre=NULL;
	
	p=*paddr;
	while(p)
	{
		if(strcmp((p->info).name,info->name)==0)
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
	if(pre==NULL)
	{
		//printf("pre==null\n");
		*paddr=(*paddr)->next;
	}
	else
	{
		//printf("pre=p->next\n");
		pre->next=p->next;
	}
	free(p);
	return 0;
}




