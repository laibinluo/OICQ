
#include "login_main.h"

int opt=1;
static struct sockaddr_in servaddr;
char readbuf[256];
static linkmessage *plm=NULL;

void  print_login_head()
{
	printf("\t\t*****************************************\n");
	printf("\t\t*****************************************\n");
	printf("\t\t*************欢迎使用本系统**************\n");
	printf("\t\t*****************************************\n");
	printf("\t\t*****************************************\n");
}


int login_connnet(struct sockaddr_in *servaddr,
					 struct hostent*server,
					 int*sock)
{
	if((server=gethostbyname(SERVER_IP))==NULL)
	{
		herror("gethostbyname");
		return RET_ERR;
	}
	if((*sock=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("socket");
		return RET_ERR;
	}
	memset(servaddr,0,sizeof(servaddr));
	servaddr->sin_family=AF_INET;
	servaddr->sin_addr.s_addr=*(uint32_t *)(server->h_addr);
	servaddr->sin_port=htons((uint16_t)PORT);

	if(connect(*sock,(struct sockaddr*) servaddr,sizeof(*servaddr))<0)
	{
		perror("connect");
		return RET_ERR;
	}
}

/*int main()
{
	login_main();
	return 0;
}
*/
linkaddr **ppaddr=NULL;
void login_main()
{
	struct sockaddr_in servaddr;
	struct hostent* server;
	int sock;
	clientInfo c_info;
	int flag;
	pack_head ph;
	int rlogin;
	int i=0;
	linkaddr *paddr=NULL;
	linkaddr *pclient=NULL;
	pthread_t id;
	pthread_t uid;
	void *result;
	int ret;
	int flag_login=-1;
	int udp_sock=-1;
	print_login_head();
	if(login_connnet(&servaddr,server,&sock)==RET_ERR)
	{
		exit(0);
	}
	login_input(&c_info);
	
	if((rlogin=login_is(sock,&c_info,&ph))==-1)
	{
		printf("\n\t\t登录失败\n");
		printf("\t\t\t姓名或密码错误\n");
		getchar();
		printf("按任意键回到主菜单");
		getchar();
		close(sock);
		return ;
	}
	else if(rlogin==-2)
	{
		printf("\n\t\t\t你已经登录\n");
		getchar();
		printf("按任意键回到主菜单");
		getchar();
		close(sock);
		return ;
	}
	
	read_link_addr(&paddr,sock,&ph);
	//print_link_client(paddr,c_info);
	ppaddr=&paddr;
	pclient=select_link_client_info(paddr,c_info);
	if(pclient==NULL)
	{
		printf("select_link_client_info error\n");
		return ;
	}
	udp_sock=manage_client_connet(pclient);
	
	ret=pthread_create(&id,NULL,(void *)thread_login,
						&sock);
	ret=pthread_create(&uid,NULL,(void *)thread_read,
						&udp_sock);
	while(1)
	{
		system("clear");
		print_login_head();
		/*printf("local host %s\n,port %d\n",
		inet_ntoa((pclient->info).addr.sin_addr),ntohs((pclient->info).addr.sin_port));*/
		printf("\n\t\t\t1.显示在线好友\n");
		printf("\t\t\t2.发送消息\n");
		printf("\t\t\t3.收消息\n");
		printf("\t\t\t4.退出\n");
		printf("\n\t\t\t请选择你要的操作\n\n");
		fflush(stdin);
		scanf("%d",&flag_login);
		fflush(stdin);
		switch(flag_login)
		{
			case 1:
				printf("\n\t\t当前在线好友:\n");
				print_link_client(paddr,c_info);
				fflush(stdin);
				getchar();
				printf("按任意键继续.....");
				getchar();
				break;
			case 2:
				fflush(stdin);
				print_send_infomation(paddr,
									  c_info,udp_sock);
				break;
			case 3:
				fflush(stdin);
				print_read_infomation(paddr,udp_sock);
				fflush(stdin);
				//getchar();
				//getchar();
				break;
			case 4:
				logout_client(sock);
				//exit(0);
				break;
			default:
				printf("\n\t\t你的选择不存在");
				printf("按任意键盘继续.....");
				fflush(stdin);
				getchar();
				break;
		}
		flag_login=-1;
	}
	
}

int login_input(clientInfo *c_info)
{
	char ch;
	
	printf("\n\n\t\t  请输入姓名:");
	scanf("%s",c_info->name);
	getchar();
	printf("\n\n\t\t\t密码:");
	scanf("%s",c_info->password);	
}
void logout_client(int sock)
{
	pack_head ph;
	ph.type=LOGOUT;
	ph.len=0;
	ph.ver=VER_TCP;
	int ret;
	
	if((ret=write(sock,&ph,sizeof(ph)))<0)
	{
		perror("ERROR writing to socket");
		//goto failed;
	}
	printf("退出\n");
	close(sock);
	exit(0);
}

int login_is(int sock,clientInfo *c_info,pack_head *ph)
{
	int ret;
		
	ph->ver=VER_TCP;
	ph->type=LOGIN;
	ph->len=sizeof(*c_info);
	
	//send pack_head
	if((ret=write(sock,ph,sizeof(ph)))<0)
	{
		//perror("ERROR writing to socket");
		goto failed;
	}
	if((ret=write(sock,c_info,sizeof(*c_info)))<0)
	{
		perror("ERROR writing to socket");
		goto failed;
	}
	if((ret=read(sock,ph,sizeof(ph)))<0)
	{
		perror("ERROR reading from socket");
		goto failed;
	}
	//printf("ver=%c;type=%c;len=%d\n",
		   //ph->ver,ph->type,ph->len);
	if(ph->type==NO)
	{
		printf("type=NO\n");
		return -1;
	}
	else if(ph->type==ONLINE)
	{
		return -2;
	}
	else
	{
		return 0;
	}

failed:
	close(sock);
	return -3;						
}

void read_link_addr(linkaddr **paddr,
					int sockfd,pack_head *ph)
{
	int i=ph->len;
	linkaddr *p=NULL;
	InfoAddr info;
	
	//printf("ph->len=%d\n",ph->len);
	if(ph->type=CLIENT_INFO)
	{
		while(i>0)
		{
			p=(linkaddr*)malloc(sizeof(linkaddr));
			if(p==NULL)
			{
				printf("malloc flase!\n");
				return ;
			}
			if((read(sockfd,&info,sizeof(info)))<0)
			{	
				free(p);
				perror("read_link_addr");
			}
			p->info=info;
			p->number=0;
			//printf("name:%s ;\n",(p->info).name);
			
			p->next=*paddr;		
			*paddr=p;
			//printf("p=%x\n",p);	
			i--;
		}
	}
	else
	{
		printf("\n\t\t姓名或密码错误!\n");
		getchar();
		printf("按任意键继续.....");
		getchar();
		return ;
	}
}

int read_new_client(linkaddr **paddr,int sock)
{
	pack_head ph;
	linkaddr *p=NULL;
	int ret;
	InfoAddr info;
	int i=0;
	//ioctl(sock,FIONBIO,&i);	
	if((ret=read(sock,&ph,sizeof(ph)))<0)
	{
		perror("ERROR writing to socket");
		goto failed;
	}
	if(ph.type==NEW)
	{
		p=(linkaddr*)malloc(sizeof(linkaddr));
		if(p==NULL)
		{
			printf("malloc flase!\n");
			return -2;
		}
	
		if((read(sock,&(p->info),sizeof(p->info)))<0)
		{	
			free(p);
			perror("read_link_addr");
		}

		p->number=0;
		p->next=*paddr;		
		*paddr=p;
		return 0;
	}
	else if(ph.type==LOGOUT)
	{	
		if((read(sock,&(info),sizeof(info)))<0)
		{	
			perror("read_link_addr");
		}
		printf("dele info->name=%s\n",info.name);
		delete_link_client(paddr,&info);
		return 0;
	}
	else
	{
		return -1;
	}
failed:
	close(sock);
	return -1;	
}

void thread_login(int *sock)
{
	
	fd_set set;
	fd_set rset;
	int nfound;	
	pack_head ph;
	ph.type=LIFE;
	ph.len=0;
	ph.ver=VER_TCP;
	FD_ZERO(&set);
	FD_SET(*sock,&set);
	struct timeval timeout;
	//printf("thread_login\n");
	
	while(1)
	{
		
		rset=set;
		timeout.tv_sec=0;
		timeout.tv_usec=500000;
		if((nfound=select(*sock+1,&rset,(fd_set*)0,
			(fd_set*)0,&timeout))<0)	
		{
			perror("select");
			return ;
		}
		else
		{
			/*if(write(*sock,&ph,sizeof(pack_head))<0)
			{
				perror("write error");
				return ;
			}
			printf("*****\n");*/
			if(FD_ISSET(*sock,&rset))
			{
				read_new_client(ppaddr,*sock);
			}
		}
		sleep(1);
	}
}

void print_read_infomation(linkaddr *paddr,int sock)
{
	linkmessage *p=NULL;
	linkmessage *q=NULL;
	int len=0;
	p=plm;
	while(p)
	{
		if(len==0)
		{
			printf("\n\t\t你有下列新消息\n");
		}
		printf("%d.姓名:%s\n",len+1,p->name);
		printf("\t\t消息:%s\n",p->message);
		printf("\n");
		q=p;
		p=p->next;
		free(q);
		len++;
	}
	plm=NULL;
	if(len==0)
	{
		printf("\n\t\t你没有新消息\n");
	}
	printf("按任意键继续....");
	getchar();
	getchar();
}

void print_send_infomation(linkaddr *paddr,
						  clientInfo info,int udp_sock)
{
	system("clear");
	print_login_head();
	char buf[BUFFER_SIZE];
	struct sockaddr_in cliaddr;
	char *name=NULL;
	int sock;
	int len=0;
	linkaddr *pclient=NULL;
	linkaddr *p=NULL;
	int flag;
	printf("\n\t\t当前在线好友:\n");
	print_link_client(paddr,info);
	printf("\n\n\t\t请选择你的在线好友\n\t\t\t");
	fflush(stdin);
	scanf("%d",&flag);
	//printf("flag=%d\n",flag);
	pclient=select_link_client(paddr,flag);
	
	//printf("********\n");
	//printf("name=%s\n",(pclient->info).name);
	if(pclient==NULL)
	{
		printf("\n\t好友不在线不能发消息\n");
		getchar();
		getchar();
		return ;
	}
	len=strlen((pclient->info).name);
	memset(buf,0,sizeof(buf));
	snprintf(buf,len+1,"%s",(info).name);
	//printf("buf=%s\n",buf);
	buf[len]=':';
	buf[len+1]=0;
	//printf("buf b=%s\n",buf);
	memset(&cliaddr,0,sizeof(servaddr));
	cliaddr.sin_family=AF_INET;
	cliaddr=(pclient->info).addr;
	//cliaddr.sin_port=htons(UDP_PROT);
	
	/*if((sock=socket(AF_INET,SOCK_DGRAM,0))<0)
	{
		perror("ERROR opening socket");
		return ;
}*/
	printf("Enter the massage:");
	//fflush(stdin);
	//fflush(stdout);
	getchar();
	fflush(stdin);
	fflush(stdin);
	if((fgets(buf+len+1,sizeof(buf)-1-len,stdin))==NULL)
	{
		if(ferror(stdin))
			perror("stdin");
		return ;
	}
	//fflush(stdin);
	if(sendto(udp_sock,buf,strlen(buf),0,
	   (struct sockaddr*)&cliaddr,sizeof(cliaddr))<0)
	{
		perror("Error sendto");
	}
	
	//printf("send host %s\n,port %d\n",
		   //inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));
	printf("\n\t\t\t发送成功!");
	printf("\n按任意键继续......");
	fflush(stdin);
	getchar();
}


int manage_client_connet(linkaddr *pclient)
{
	//printf("------\n");
	int upd_sockfd=-1;
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	//servaddr=(pclient->info).addr;
	servaddr=(pclient->info).addr;
	
	//printf("manage_client\n");
	//printf("local host %s\n,port %d\n",
		   //inet_ntoa(servaddr.sin_addr),ntohs(servaddr.sin_port));
	if((upd_sockfd=socket(AF_INET,SOCK_DGRAM,0))<0)
	{
		perror("socket");
		return -1;
	}
	//printf("udp_sockfd=%d\n",upd_sockfd);
	/*if(setsockopt(upd_sockfd,SOL_SOCKET,SO_REUSEADDR,
	   &opt,sizeof(opt))<0)
	{
		perror("ERROR setsockopt");
		return -1;
}*/
	
	if(bind(upd_sockfd,(struct sockaddr *)&servaddr,
	   sizeof(servaddr))<0)
	{
		perror("bind");
		return -1;
	}
	//printf("upd_sockfd=%d\n",upd_sockfd);
	return upd_sockfd;
}

void thread_read(int *sock)
{
	int ret;
	char buf[BUFFER_SIZE];
	char name[20];
	char *p;
	int i=0;
	int udp_sock=*sock;
	struct sockaddr_in recaddr;
	linkaddr *pclient=NULL;
	linkmessage *pnm=NULL; 
	
	while(1)
	{
		i=0;
		int len=sizeof(recaddr);
		if((ret=recvfrom(udp_sock,readbuf,
			sizeof(readbuf),0,
			(struct sockaddr*)&(recaddr),&len))<0)
		{
			perror("Error sendto");
		}
		readbuf[ret]=0;

		p=readbuf;
		if(strlen(readbuf)==0)
		{
			continue;
		}
		
		pnm=(linkmessage*)malloc(sizeof(linkmessage));
		while(*p!=':')
		{
			name[i]=*p;
			i++;
			p++;
		}
		name[i]=0;
		//printf("name=%s\n",name);
		strcpy(pnm->name,name);
		p++;
		strcpy(pnm->message,p);
		
		pnm->next=plm;
		plm=pnm;
		memset(readbuf,0,sizeof(buf));
		memset(name,0,sizeof(name));
	}
	
}

