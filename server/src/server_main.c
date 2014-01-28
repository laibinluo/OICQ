#include "server_main.h"

static fd_set rset;
static fd_set set;
static linkinfo  *pinfo=NULL;//=ReadFile();
static	linkaddr *paddr=NULL;
static	linkinfo *pclient=NULL;
static	linkaddr *pout=NULL;
static	t_client *pt=NULL;
static int request_sock;

/*void write_link_addr(linkaddr *paddr,linkinfo *pclient,int sockfd)
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
						 linkinfo *pclient,char type)
{
	pack_head ph;
	linkaddr *p=paddr;
	ph.ver=VER_TCP;
	ph.type=type;
	ph.len=1;
	linkaddr* pnew=select_link_name(paddr,
							(pclient->info).name);
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
		if(strcmp((p->info).name,(pclient->info).name)==0)
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
*/
int server_listen()
{	
	struct sockaddr_in servaddr;
	int request_sock; 
	uint32_t addrlen;	
		
	if((request_sock=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("socket");
		return -1;
	}
	
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=INADDR_ANY;
	servaddr.sin_port=htons((uint16_t)PORT);

	if(bind(request_sock,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
	{
		perror("bind");
		return -1;
	}

	if(listen(request_sock,LISTEN_QUEUE_NUM)<0)
	{
		perror("listen");
		return -1;
	}
	return request_sock;	
}

int main()
{
	server_main();
	return 0;
}

void server_main()
{
	struct sockaddr_in remote;
	int new_sock;
	int nfound;
	int fd;
	int maxfd;
	int bytesread;
	uint32_t addrlen;
	t_client *ptc=NULL;
	struct timeval timeout;
	pack_head ph;
	clientInfo info;
	
	request_sock=server_listen();
	
	time_t t;
	pthread_t id;
	read_link_info(&pinfo);
	
	char type;
	
	FD_ZERO(&set);
	FD_SET(request_sock,&set);
	maxfd=request_sock;
	
	/*if(pthread_create(&id,NULL,(void*)is_active_client,
	   NULL)<0)
	{
		printf("create pthread error\n");
		return ;
	}
	*/
	while(1)
	{
		rset=set;
		timeout.tv_sec=0;
		timeout.tv_usec=500000;

		if((nfound=select(maxfd+1,&rset,(fd_set*)0,(fd_set*)0,&timeout))<0)
		{
			perror("select\n");
			return ;
		}
		else
		{
			//printf("nfount=%d\n",nfound);
			if(nfound==0)
			{
				//print_link_info(pinfo);
				printf(".");
				fflush(stdout);
				continue;
			}
		}	
		printf("nfound=%d\n",nfound);
		if(FD_ISSET(request_sock,&rset))
		{
			addrlen=sizeof(remote);
			if((new_sock=accept(request_sock,(struct sockaddr*)&remote,
				&addrlen))<0)
			{
				perror("accept");
				return ;
			}		
			printf("connection from host %s\n,port %d\n,socket %d\r\n",
				   inet_ntoa(remote.sin_addr),ntohs(remote.sin_port),
			new_sock);
			add_link_addr(&paddr,remote,new_sock);	
			
			FD_SET(new_sock,&set);

			if(new_sock>maxfd)
			{
				maxfd=new_sock;
			}
			FD_CLR(request_sock,&rset);
			nfound--;
		}
		for(fd=0;fd<=maxfd&&nfound>0;fd++)
		{
			if(FD_ISSET(fd,&rset))
			{
				nfound--;	
				
				type=read_pack_head(fd,&ph);
				pclient=check_info(fd,type,
					&pinfo,&info,paddr);
				printf("type=%c\n",type);
				
				if(type==LOGOUT)
				{
					
					delete_link_addr(&paddr,fd);
					
					//printf("LOGOUT\n");
					fprintf(stderr,"server:end of file on %d\r\n",fd);
					FD_CLR(fd,&set);
					close(fd);
					continue;		
				}
				else if(type==LOGIN)
				{
					//add_time_link(fd,&pt);
				}
				printf("read_pack_head after\n");
				//t=time(NULL);
				//ptc=selete_time_link(fd,pt);
				//ptc->t_begin=t;		
			}
		}
	}
	return ;
}

void is_active_client()
{
	t_client *ptc=NULL;
	t_client *pre=NULL;
	time_t t;
	int sock;
	linkaddr* pnew=NULL;
	int second;
	
	while(1)
	{
		ptc=pt;
		while(ptc)
		{
			t=time(NULL);
			second=t-(ptc->t_begin);
			sock=ptc->sock;
			ptc=ptc->next;
			printf("second=%d\n",second);
			
			if(second>3)
			{
				pnew=select_link_addr(paddr,sock);
				write_new_addr_login(paddr,
						 (pnew->info).name,LOGOUT);
				delete_link_addr(&paddr,sock);
					
				//printf("LOGOUT\n");
				FD_CLR(sock,&set);
				close(sock);
				delete_time_link(sock,&pt);
				printf("delet sock\n");
				break;	
			}
		}
		sleep(1);
	}
}
