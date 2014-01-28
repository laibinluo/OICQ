
#include "login_main.h"
#include "register_main.h"

void  print_head()
{
	printf("\t\t*****************************************\n");
	printf("\t\t*****************************************\n");
	printf("\t\t*************欢迎使用本系统**************\n");
	printf("\t\t*****************************************\n");
	printf("\t\t*****************************************\n");
}

void client_main()
{
	int flag=0;
	char ch;
	while(1)
	{
		system("clear");
		print_head();

		printf("\n\n\t请选择你要的操作\n");
		printf("\t\t\t1.用户注册\n");
		printf("\t\t\t2.用户登录\n");
		printf("\t\t\t3.退出\n");
		
		flag=-1;
		scanf("%d",&flag);
   
		switch(flag)
		{
			case 1:
				register_main();
				break;
			case 2:
				login_main();
				break;
			case 3:
				//logout_main();
				exit(0);
			default:
				printf("\t你选择的操作不存在，请从新选择\n");
				printf("\n按任意键继续.........");
				getchar();
				scanf("%c",&ch);
				break;
		}  
	}	
}

int main()
{
	client_main();
	return 0;
}

