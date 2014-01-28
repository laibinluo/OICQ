
#include "linkinfo.h"

void creat_link_info(linkinfo **pinfo)
{
	//linkinfo *phead=NULL;
	linkinfo *p=NULL;
	clientInfo c_info;
	//phead=*pinfo;
	
	printf("name:");
	scanf("%s",c_info.name);
	
	while(strcmp(c_info.name,"#")!=0)
	{
		printf("password:");
		scanf("%s",c_info.password);
		p=(linkinfo*)malloc(sizeof(linkinfo));
					
		p->info=c_info;
		p->next=(*pinfo);
		(*pinfo)=p;
		
		printf("name:");
		scanf("%s",c_info.name);
	}
	
}

void print_link_info(const linkinfo *pinfo)
{
	const linkinfo *p=pinfo;
	
	while(p)
	{
		printf("name:%s ",(p->info).name);
		printf("password:%s ",(p->info).password);
		printf("\n");
		p=p->next;
	}
}


void free_link_info(linkinfo **pinfo)
{
	linkinfo *p=NULL;
	
	p=*pinfo;
	while(*pinfo!=NULL)
	{
		(*pinfo)=(*pinfo)->next;
		free(p);
		p=*pinfo;
	}
}


void save_client_info(linkinfo* pinfo)
{
	if(pinfo==NULL)
	{
		return ;
	}
	FILE *fp=NULL;
	linkinfo *p=pinfo;
	
	if((fp=fopen(PATH_CLIENT,"w"))==NULL)
	{
		perror("fopen file");
		return ;
	}
	
	while(p!=NULL)
	{
		if(fwrite(p,sizeof(linkinfo),1,fp)!=1)
		{
			fclose(fp);
			return ;
		}	
		p=p->next;
	}
	
	fclose(fp);
}
	
void read_link_info(linkinfo **pinfo)
{
	FILE *fp=NULL;
	linkinfo *p=NULL;
	
	if((fp=fopen(PATH_CLIENT,"r"))==NULL)
	{
		perror("fopen file");
		return ;
	}
	
	while(!feof(fp))
	{
		p=(linkinfo*)malloc(sizeof(linkinfo));
			
		if(p==NULL)
		{
			printf("memrroy false\n");
			return ;
		}
		if(fread(p,sizeof(linkinfo),1,fp)!=1)
		{
			fclose(fp);
			free(p);
			return ;
		}	
		p->next=*pinfo;
		*pinfo=p;
	}
	
	fclose(fp);
}
	
	
int add_link_info(linkinfo **pinfo,clientInfo info)
{
	linkinfo *p=NULL;
	p=*pinfo;
	
	while(p!=NULL)
	{
		if(strcmp((p->info).name,info.name)==0)
		{
			return -1;
		}
		p=p->next;
	}
	p=(linkinfo *)malloc(sizeof(linkinfo));
	p->info=info;
	p->next=*pinfo;
	*pinfo=p;
	return 0;
}	
	
linkinfo *select_link_info(linkinfo *pinfo,clientInfo info)
{
	linkinfo *p=pinfo;
	
	while(p!=NULL)
	{
		printf("*****\n");
		if((strcmp((p->info).name,info.name)==0)&&
				  (strcmp((p->info).password,
				   info.password)==0))
		{
			return p;		
		}
		p=p->next;
	}
	return NULL;
}	
	
/*int main()
{
	linkinfo *pinfo=NULL;
	clientInfo c_info;
	read_link_info(&pinfo);
	//creat_link_info(&pinfo);
	/*do
	{
		printf("name:");
		scanf("%s",c_info.name);
		printf("password:");
		scanf("%s",c_info.password);
			
}while(add_link_info(&pinfo,c_info));*/
	
	/*printf("name:");
	scanf("%s",c_info.name);
	printf("password:");
	scanf("%s",c_info.password);
	linkinfo *p=select_link_info(pinfo,c_info);
	printf("select after\n");
	if(p!=NULL)
	{
		printf("name:%s ",(p->info).name);
		printf("password:%s ",(p->info).password);
		printf("\n");
	}
	else
	{
		printf("not found\n");
	}
	//pmainrint_link_info(pinfo);
	save_client_info(pinfo);
	free_link_info(&pinfo);
	return 0;
}*/
	








