#include "server_check.h"


void read_client_info(int fd,clientInfo *info)
{
	int ret;
	
	if((ret=read(fd,info,sizeof(*info)))<0)
	{
		perror("read");
	}
}

char read_pack_head(int fd,pack_head *ph)
{
	int ret;

	if((ret=read(fd,ph,sizeof(*ph)))<0)
	{
		perror("read");
	}
	
	if(ret==0)
	{
		fprintf(stderr,"serve:end of file on:%d\r\n",fd);
		return LOGOUT;
	}
	printf("ver=%c;type=%c;len=%d\n",
		   ph->ver,ph->type,ph->len);
	return ph->type;
}

void write_pack_head(int fd,pack_head *ph)
{	
	int ret;
	if((ret=write(fd,ph,sizeof(pack_head)))<0)
	{
		perror("write_pack_head");
	}
	printf("write_pack_head success\n");
}

void is_register(int fd,linkinfo **pinfo,clientInfo *info)
{
	pack_head ph;
	ph.ver=VER_TCP;
	ph.len=0;
	
	read_client_info(fd,info);
	//printf("name=%s ;password=%s\n",
		  // info->name,info->password);
	
	if(add_link_info(pinfo,*info)==-1)
	{
		ph.type=NO;
	}
	else
	{
		ph.type=YES;
		save_client_info(*pinfo);
	}
	print_link_info(*pinfo);
	write_pack_head(fd,&ph);
}


linkinfo* is_login(int fd,linkinfo *pinfo,
				   clientInfo *info,linkaddr *paddr)
{
	pack_head ph;
	ph.ver=VER_TCP;
	ph.len=0;
	linkaddr *ponline=NULL;
	linkinfo *pclient=NULL;
	
	read_client_info(fd,info);
	pclient=select_link_info(pinfo,*info);
	ponline=select_link_name(paddr,info->name);
	
	if(pclient==NULL)
	{
		ph.type=NO;
	}
	else
	{
		if(ponline!=NULL)
		{
			printf("online\n");
			ph.type=ONLINE;
			pclient=NULL;
		}
		else
		{
			
			ph.type=CLIENT_INFO;
			ph.len=lenght_client_info(paddr,
								  &(pclient->info),fd);
			printf("ph.len=%d\n",ph.len);
		}
	}
	write_pack_head(fd,&ph);
	return pclient;
}

int lenght_client_info(linkaddr *paddr,clientInfo *info,int sockfd)
{
	linkaddr *p=paddr;
	int lenght=0;
	
	while(p)
	{
		if(strlen((p->info).name)==0)
		{
			if(p->sockfd==sockfd)
			{
				strcpy((p->info).name,info->name);
				lenght++;
			}
			p=p->next;
			continue;
		}
		lenght++;
		p=p->next;
	}
	return lenght;
}

void is_error(int fd)
{
	pack_head ph;
	ph.ver=VER_TCP;
	ph.len=0;
	ph.type=NO;
	write_pack_head(fd,&ph);
	
}

linkinfo* check_info(int fd,char flag,linkinfo **pinfo,
					 clientInfo *info,linkaddr *paddr)
{
	linkinfo* pclient=NULL;
	linkinfo outinfo;
	linkaddr *pout=NULL;
	switch(flag)
	{
		case REGISTER:
			is_register(fd,pinfo,info);
			break;
			
		case LOGIN:
			printf("read_pack_head after\n");
			pclient=is_login(fd,*pinfo,info,paddr);
			
			if(pclient!=NULL)
			{
				write_link_addr(paddr,fd);
				write_new_addr_login(paddr,
							(pclient->info).name,NEW);
			}
			printf("*********\n");
			break;
		case LOGOUT:
			pout=select_link_addr(paddr,fd);
			//printf("(pout->info).name=%s\n",
				  // (pout->info).name);
			write_new_addr_login(paddr,
							(pout->info).name,LOGOUT);
			break;
		case CLOSED:
			break;
		default:
			is_error(fd);
			break;
	}
	return pclient;
}

void write_link_addr(linkaddr *paddr,int sockfd)
{
	linkaddr *p=paddr;
	InfoAddr info;
	int len=lenght_link_addr(paddr);
	printf("len=%d\n",len);
	
	while(p!=NULL&&len>0)
	{
		if(strlen((p->info).name)==0)
		{
			p=p->next;
			continue;
		}
		info=p->info;
		if(write(sockfd,&info,sizeof(info))<0)
		{
			perror("write_link_addr");
			return ;
		}
		//printf("len=%d\n",len);
		len--;
		p=p->next;
	}
	
}

void write_new_addr_login(linkaddr *paddr,
						  char *name,char type)
{
	pack_head ph;
	linkaddr *p=paddr;
	ph.ver=VER_TCP;
	ph.type=type;
	ph.len=1;
	if(strlen(name)==0)
	{
		return ;
	}
	linkaddr* pnew=select_link_name(paddr,name);
	if(pnew==NULL)
	{
		return ;
	}
	while(p!=NULL)
	{
		if(strlen((p->info).name)==0)
		{
			//printf("strlen\n");
			p=p->next;
			continue;
		}
		//printf("writ_link_addr2:%s\n",(p->info).name);
		if(strcmp((p->info).name,name)==0)
		{
			//printf("strcmp\n");
			p=p->next;
			continue;
		}
		//printf("write\n");
		write_pack_head(p->sockfd,&ph);
		write(p->sockfd,&(pnew->info),
			  sizeof(pnew->info));
		p=p->next;
	}
}


void  add_time_link(int sock,t_client **ptt)
{
	t_client *pt=NULL;
	time_t t1=time(NULL);
	
	pt=(t_client*)malloc(sizeof(t_client));
	pt->t_begin=t1;
	pt->sock=sock;
				
	pt->next=*ptt;
	*ptt=pt;			
}
t_client* selete_time_link(int sock,t_client*pt)
{
	t_client *tc=pt;
	
	while(tc)
	{
		if(tc->sock==sock)
			break;
		tc=tc->next;
	}
	return tc;
}	
int delete_time_link(int sock,t_client **pt)
{
	t_client *tc=NULL;
	t_client *pretc=NULL;
	tc=*pt;
	
	while(tc)
	{
		if(tc->sock==sock)
			break;
		pretc=tc;
		tc=tc->next;
	}
	if(tc==NULL)
	{
		return -1;
	}
	if(pretc==NULL)
	{
		*pt=(*pt)->next;
	}
	else
	{
		pretc=tc->next;
	}
	free(tc);
	return 0;
}	

