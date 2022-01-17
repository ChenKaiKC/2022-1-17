#include "inc.h"


int main(int argc, const char *argv[])
{
	if(argc < 3)
	{
		printf("传参错误\n");
		return -1;
	}

	int sockfd;
	int res;
	res = create_socket_cil(&sockfd,argv);
	if(res == -1)
	{
		return -1;
	}
	else
	{
		view_1(&sockfd);
	}
	return 0;
}

int create_socket_cil(int *sockfd,const char *argv[])
{
	//创建套接字
	*sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(*sockfd < 0)
	{
		ERRMSG("socket");
		return -1;
	}

	//端口快速重用
	int reuse = 1;
	if(setsockopt(*sockfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse)) < 0)
	{
		ERRMSG("setsockopt");
		return -1;
	}

	//链接服务器
	struct sockaddr_in sin;
	int port;
	port = ((*argv[2] - '0') *1000 +\
			(*(argv[2] + 1) - '0') * 100 +\
			(*(argv[2] + 2) - '0') * 10 +\
			(*(argv[2] + 3) - '0') * 1\
			);
	sin.sin_family          = AF_INET;
	sin.sin_port            = htons(port);
	sin.sin_addr.s_addr     = inet_addr(argv[1]);

	if(connect(*sockfd,(struct sockaddr *)&sin,sizeof(sin)) < 0)
	{
		ERRMSG("connect");
		return -1;
	}

	return *sockfd;

}
//员工管理员公用注册
int do_register_staff_cil(int *sockfd,int power)
{
	char buf[64] ="";
	char phone[14] ="";
	int i;
	//电话号码作为用户名使用
	while(1)
	{
		printf("请输入你的电话号码:");
		fflush(stdout);
		scanf("%s",phone);
		getchar();
		for(i=0;i<13;)
		{
			if(phone[i] >= '0'&&phone[i] <= '9'&&phone[13] == 0)
			{
				i++;
			}
			else
			{
				printf("格式错误\n");
				while(getchar() != '\n');
				break;
			}
		}
		if(i == 13)
		{
			break;
		}
	}
	if(power == 0)
	{
		strcpy(buf,REG_STAFF);
	}
	else if(power == 1)
	{
		strcpy(buf,REG_ADMIN);
	}
	strcat(buf,phone);
	if(send(*sockfd,buf,sizeof(buf),0) < 0)
	{
		ERRMSG("send");
		return -1;
	}

	if(recv(*sockfd,buf,sizeof(buf),0) < 0)
	{
		ERRMSG("recv");
		return -1;
	}
	if(buf[0] == '1')
	{
	//发送注册的密码
	bzero(buf,sizeof(buf));
	printf("请输入你的密码:");
	fflush(stdout);
	scanf("%s",buf);
	getchar();
	if(send(*sockfd,buf,sizeof(buf),0) < 0)
	{
		ERRMSG("send");
		return -1;
	}
	}
	else
	{
		printf("%s\n",buf);
		return -1;
	}
	return 0;
}

int do_login_staff_cil(int *sockfd,int power)
{
	char buf[64] ="";
	char phone[14] ="";
	int i;
	//电话号码作为用户名使用
	while(1)
	{
		printf("请输入你的电话号码:");
		fflush(stdout);
		scanf("%s",phone);
		getchar();
		for(i=0;i<13;)
		{
			if(phone[i] >= '0'&&phone[i] <= '9'&&phone[13] == 0)
			{
				i++;
			}
			else
			{
				printf("格式错误\n");
				while(getchar() != '\n');
				break;
			}
		}
		if(i == 13)
		{
			break;
		}
	}
	if(power == 0)
	{
		strcpy(buf,LOG_STAFF);
	}
	else if(power == 1)
	{
		strcpy(buf,LOG_ADMIN);
	}
	strcat(buf,phone);

	if(send(*sockfd,buf,sizeof(buf),0) < 0)
	{
		ERRMSG("send");
		return -1;
	}
	
	if(recv(*sockfd,buf,sizeof(buf),0) < 0)
	{
		ERRMSG("recv");
		return -1;
	}
	if(buf[0] == '1')
	{
	//发送登录的密码
	bzero(buf,sizeof(buf));
	printf("请输入你的密码:");
	fflush(stdout);
	scanf("%s",buf);
	getchar();
	if(send(*sockfd,buf,sizeof(buf),0) < 0)
	{
		ERRMSG("send");
		return -1;
	}
	}
	else
	{
		printf("%s\n",buf);
		return -1;
	}

	if(recv(*sockfd,buf,sizeof(buf),0) < 0)
	{
		ERRMSG("recv");
		return -1;
	}
	if(buf[0] == '1'&&power == 0)
	{
		//view_2();
		view_staff(sockfd,phone);
	}
	else if(buf[0] == '1'&&power == 1)
	{
		//view_3();
		view_admin(sockfd,phone);
	}
	else
	{
		printf("%s\n",buf);
		return -1;
	}

	return 0;
}

