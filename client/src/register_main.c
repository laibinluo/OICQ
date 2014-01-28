
#include "register_main.h"

void  print_register_head()
{
	printf("\t\t*****************************************\n");
	printf("\t\t*****************************************\n");
	printf("\t\t*************欢迎使用本系统**************\n");
	printf("\t\t*****************************************\n");
	printf("\t\t*****************************************\n");
}

void register_main()
{
	struct sockaddr_in servaddr;
	struct hostent* server;
	int sock;
	clientInfo c_info;
	int flag;
	
	print_head();
	if(register_connnet(&servaddr,server,&sock)==RET_ERR)
	{
		exit(0);
	}	
	
	
	while(1)
	{
		system("clear");
		print_head();
		register_input(&c_info);
		
		if(!register_is(sock,&c_info))
		{
			printf("\t\t注册成功\n");
			printf("按任意键回到主菜单\n");
			close(sock);
			getchar();
			getchar();
			return ;
		}
		printf("姓名必须唯一\n");
		getchar();
		printf("\t\t是否继续注册?y/n ");
		if(getchar()!='y')
			break;
		while(getchar()!='\n');
	}
	//scanf("%d",&flag);
}

int register_connnet(struct sockaddr_in *servaddr,
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


int register_input(clientInfo *c_info)
{
	char ch;
	
	printf("\n\n\t\t  请输入姓名:");
	scanf("%s",c_info->name);
	getchar();
	printf("\n\n\t\t\t密码:");
	scanf("%s",c_info->password);	
}

int register_is(int sock,clientInfo *c_info)
{
	int ret;
	pack_head ph;
	
	ph.ver=VER_TCP;
	ph.type=REGISTER;
	ph.len=sizeof(*c_info);
	
	//send pack_head
	if((ret=write(sock,&ph,sizeof(ph)))<0)
	{
		perror("ERROR writing to socket");
		goto failed;
	}
	if((ret=write(sock,c_info,sizeof(*c_info)))<0)
	{
		perror("ERROR writing to socket");
		goto failed;
	}
	if((ret=read(sock,&ph,sizeof(ph)))<0)
	{
		perror("ERROR reading from socket");
		goto failed;
	}
	//printf("ver=%c;type=%c;len=%d\n",ph.ver,ph.type,ph.len);
	if(ph.type==YES)
	{
		return 0;
	}
	else
	{
		return -1;
	}

failed:
	close(sock);
	return -2;						
}

/*int main()
{
	register_main();
}*/

