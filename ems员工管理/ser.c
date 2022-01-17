#include "inc.h"

int main(int argc, const char *argv[])
{
	if(argc < 3)
	{
		printf("传参错误\n");
		return -1;
	}
	//创建套接字
	int sockfd;
	int res;
	res = create_socket_ser(&sockfd,argv);
	if(res == -1)
	{
		return -1;
	}

	//导入员工表
	sqlite3 *efpd;
	import_form(&efpd); 
	
	//多线程并发服务器
	struct sockaddr_in sin1;
	socklen_t len = sizeof(sin1);
	int newfd;
	pthread_t pid;
	parameter ptr;
	ptr.efpd = efpd;
	while(1)
	{
		//建立连接
		newfd = accept(sockfd,(struct sockaddr *)&sin1,&len);
		if(newfd < 0)
		{
			ERRMSG("accept");
			return -1;
		}

		//创建线程
		ptr.newfd = newfd;
		if(pthread_create(&pid,NULL,myrecv,(void *)&ptr) < 0)
		{
			ERRMSG("pthread_create");
			return -1;
		}
	}

	return 0;
}

//创建套接字
int create_socket_ser(int *sockfd,const char *argv[])
{
	int reuse = 1;
	unsigned short port;
	struct sockaddr_in sin;
	
	
	//创建套接字
	*sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(*sockfd < 0)
	{
		ERRMSG("sockfd");
		return -1;
	}

	//端口快速重用
	if(setsockopt(*sockfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse)) < 0)
	{
		ERRMSG("setsockopt");
		return -1;
	}

	//绑定端口
	//外部传参的第三个参数是端口号
	port = ((*argv[2] - '0') * 1000 +\
			(*(argv[2]+1) - '0') * 100 +\
			(*(argv[2]+2) - '0') * 10 +\
			(*(argv[2]+3) - '0') * 1
			);
	sin.sin_family      = AF_INET;
	sin.sin_port        = htons(port);
	sin.sin_addr.s_addr = inet_addr(argv[1]);
	if(bind(*sockfd,(struct sockaddr *)&sin,sizeof(sin)) < 0)
	{
		ERRMSG("bind");
		return -1;
	}

	//监听模式
	if(listen(*sockfd,20) < 0)
	{
		ERRMSG("listen");
		return -1;
	}
	return *sockfd;
}


void *myrecv(void *arg)
{
	pthread_detach(pthread_self());
	parameter ptr = *(parameter *)arg;
	char phone[64] = "";
//	char name[64] = "";
	char buf[128] = "";
	char inform[64] = "";
	int res = 1;
	char *errmsg;
	while(1)
	{
		//接受信息
		if((res = recv(ptr.newfd,inform,sizeof(inform),0)) < 0)
		{
			ERRMSG("recv");
			return NULL;
		}//判断异常退出
		else if(res == 0)
		{
			if(phone[0] == 0)
			{
				printf("客户端退出\n");
				break;
			}
			bzero(buf,sizeof(buf));
			sprintf(buf,"update staff set type=0 where phone=\"%s\";",phone);
			if(sqlite3_exec(ptr.efpd,buf,NULL,NULL,&errmsg) != SQLITE_OK)
			{
				printf("__%d__%s\n",__LINE__,errmsg);
				return NULL;
			}
			printf("员工%s客户端退出\n",phone);
	//		bzero(name,sizeof(name));
			bzero(phone,sizeof(phone));
			break;
		}
	
		//
		switch(inform[0])
		{
		case '0':
			bzero(buf,sizeof(buf));
			sprintf(buf,"update staff set type=0 where phone=\"%s\";",inform+1);
			if(sqlite3_exec(ptr.efpd,buf,NULL,NULL,&errmsg) != SQLITE_OK)
			{
				printf("__%d__%s\n",__LINE__,errmsg);
				return NULL;
			}
			printf("员工%s客户端退出\n",inform+1);
			break;
			//员工注册
		case '1':
			do_register_staff_ser(inform,&ptr,0);
			bzero(inform,sizeof(inform));
			break;
			//管理员注册
		case '2':
			do_register_staff_ser(inform,&ptr,1);
			bzero(inform,sizeof(inform));
			break;
			//员工登录
		case '3':
			do_login_staff_ser(inform,&ptr,0);
			bzero(inform,sizeof(inform));
			break;
			//管理员登录
		case '4':
			do_login_staff_ser(inform,&ptr,1);
			bzero(inform,sizeof(inform));
			break;
			//查看个人信息
		case '5':
			check_self_ser(inform,&ptr);
			bzero(inform,sizeof(inform));
			break;
			//更改个人信息
		case '6':
			change_self_ser(&ptr.newfd,inform,&ptr);
			bzero(inform,sizeof(inform));
			break;
		case '7':
			check_self_ser(inform,&ptr);
			bzero(inform,sizeof(inform));
			break;
		case '8':
			change_self_ser(&ptr.newfd,inform,&ptr);
			bzero(inform,sizeof(inform));
			break;
		case '9':
			do_delete(&ptr.newfd,inform,&ptr);
			bzero(inform,sizeof(inform));
			break;
		default:
			printf("异常\n");
			break;
		}

	}
}

int do_register_staff_ser(char *inform,parameter *ptr,int power)
{
	char buf[128] ="";
	char err[64] ="";
	char passwd[64] ="";
	char *errmsg;

	sprintf(buf,"insert into staff(phone,type,power) values(\"%s\",0,%d);",inform+1,power);
	if(sqlite3_exec(ptr->efpd,buf,NULL,NULL,&errmsg) != SQLITE_OK)
	{
		if(sqlite3_errcode(ptr->efpd) == 19)
		{
			bzero(err,sizeof(err));
			sprintf(err,"手机号已存在\n");
			if(send(ptr->newfd,err,sizeof(err),0) < 0)
			{
				ERRMSG("send");
				return -1;
			}
			return -2;
		}
	}
		err[0] = '1';
		if(send(ptr->newfd,err,sizeof(err),0) < 0)
		{
			ERRMSG("send");
			return -1;
		}
		if(recv(ptr->newfd,passwd,sizeof(passwd),0) < 0)
		{
			ERRMSG("recv");
			return -1;
		}
		printf("密码%s\n",passwd);
		printf("手机%s\n",inform);
		bzero(buf,sizeof(buf));
		sprintf(buf,"update staff set passwd=\"%s\" where phone=\"%s\";",passwd,inform+1);
		if(sqlite3_exec(ptr->efpd,buf,NULL,NULL,&errmsg) != SQLITE_OK)
		{
			printf("__%d__%s\n",__LINE__,errmsg);
			return -1;
		}
	
	return 0;
}


int do_login_staff_ser(char *inform,parameter *ptr,int power)
{
	char buf[128] ="";
	char err[64] ="";
	char passwd[64] ="";
	char **presult;
	char *errmsg;
	int line = 0;
	int row = 0;

	sprintf(buf,"select * from staff where phone=\"%s\" and power=%d;",inform+1,power);
	if(sqlite3_get_table(ptr->efpd,buf,&presult,&line,&row,&errmsg) != SQLITE_OK)
	{
		printf("__%d__%s\n",__LINE__,errmsg);
		return -1;
	}
	if(line == 0)
	{
		bzero(err,sizeof(err));
		sprintf(err,"手机号不存在\n");
		if(send(ptr->newfd,err,sizeof(err),0) < 0)
		{
			ERRMSG("send");
			return -1;
		}
		return -1;
	}
	if(*presult[14] == 1)
	{
		bzero(err,sizeof(err));
		sprintf(err,"用户已经被登录\n");
		if(send(ptr->newfd,err,sizeof(err),0) < 0)
		{
			ERRMSG("send");
			return -1;
		}
		return -1;
	}
		err[0] = '1';
		if(send(ptr->newfd,err,sizeof(err),0) < 0)
		{
			ERRMSG("send");
			return -1;
		}
		if(recv(ptr->newfd,passwd,sizeof(passwd),0) < 0)
		{
			ERRMSG("recv");
			return -1;
		}
		printf("密码%s\n",passwd);
		printf("手机%s\n",inform);
		if(strcmp(presult[10],passwd) == 0)
		{
		bzero(buf,sizeof(buf));
		sprintf(buf,"update staff set type=1 where phone=\"%s\";",inform+1);
		if(sqlite3_exec(ptr->efpd,buf,NULL,NULL,&errmsg) != SQLITE_OK)
		{
			printf("__%d__%s\n",__LINE__,errmsg);
			return -1;
		}
		if(send(ptr->newfd,err,sizeof(err),0) < 0)
		{
			ERRMSG("send");
			return -1;
		}
		}
		else
		{
			bzero(err,sizeof(err));
			sprintf(err,"密码错误\n");
			if(send(ptr->newfd,err,sizeof(err),0) < 0)
			{
				ERRMSG("send");
				return -1;
			}
			return -1;
		}

	return 0;
}

int check_self_ser(char *inform,parameter *ptr)
{
	char buf[128] ="";
	char **presult;
	char *errmsg;
	int line = 0;
	int row = 0;
	message msg;
	sprintf(buf,"select * from staff where phone=\"%s\";",inform+1);
	if(sqlite3_get_table(ptr->efpd,buf,&presult,&line,&row,&errmsg) != SQLITE_OK)
	{
		printf("__%d__%s\n",__LINE__,errmsg);
		return -1;
	}	
	if(line == 0)
	{
		memset(&msg,0,sizeof(msg));
		sprintf(msg.name,"%d手机号不存在\n",7);
		if(send(ptr->newfd,&msg,sizeof(msg),0) < 0)
		{
			ERRMSG("send");
			return -1;
		}
		return -1;
	}
	else
	{
		memset(&msg,0,sizeof(msg));

		//打包
		make_message(&msg,presult);
		//发包
		send_message_ser(ptr,&msg);
	}

	return 0;
}
int do_delete(int *sockfd,char *inform,parameter *ptr)
{
	char buf[128] ="";
	char phone[64] ="";
	char **presult;
	char *errmsg;
	int line = 0;
	int row = 0;
	message msg;
	message msg2;
	sprintf(buf,"select * from staff where phone=\"%s\";",inform+1);
	if(sqlite3_get_table(ptr->efpd,buf,&presult,&line,&row,&errmsg) != SQLITE_OK)
	{
		printf("__%d__%s\n",__LINE__,errmsg);
		return -1;
	}	
	if(line == 0)
	{
		memset(&msg,0,sizeof(msg));
		sprintf(msg.name,"%d手机号不存在\n",7);
		if(send(ptr->newfd,&msg,sizeof(msg),0) < 0)
		{
			ERRMSG("send");
			return -1;
		}
		return -1;
	}
	else
	{

		bzero(buf,sizeof(buf));
		sprintf(buf,"delete from staff where phone=\"%s\";",inform+1);
		if(sqlite3_exec(ptr->efpd,buf,NULL,NULL,&errmsg) != SQLITE_OK)
		{
			msg2.name[0] = 3;
			send_message_ser(ptr,&msg2);
			printf("__%d__%s\n",__LINE__,errmsg);
			return -1;
		}
		else
		{	
		//发包
			msg2.name[0] = 1;
			send_message_ser(ptr,&msg2);
		}
	}

	return 0;
}

int change_self_ser(int *sockfd,char *inform,parameter *ptr)
{
	char buf[128] ="";
	char phone[64] ="";
	char **presult;
	char *errmsg;
	int line = 0;
	int row = 0;
	message msg;
	message msg2;
	sprintf(buf,"select * from staff where phone=\"%s\";",inform+1);
	if(sqlite3_get_table(ptr->efpd,buf,&presult,&line,&row,&errmsg) != SQLITE_OK)
	{
		printf("__%d__%s\n",__LINE__,errmsg);
		return -1;
	}	
	if(line == 0)
	{
		memset(&msg,0,sizeof(msg));
		sprintf(msg.name,"%d手机号不存在\n",7);
		if(send(ptr->newfd,&msg,sizeof(msg),0) < 0)
		{
			ERRMSG("send");
			return -1;
		}
		return -1;
	}
	else
	{
		make_message(&msg2,presult);
		printf("msg2%s\n",msg2.phone);
		memset(&msg,0,sizeof(msg));
		if(recv(*sockfd,&msg,sizeof(msg),0) < 0)
		{
			ERRMSG("recv");
			return -1;
		}
		printf("msg%s\n",msg.phone);
		strcpy(phone,msg2.phone);
		update_message_ser(&msg,&msg2);
		strcpy(msg.phone,phone);
		
		if(change_message_ser(&msg2,ptr,&msg) == 0)
		{
		//发包
		msg2.name[0] = 1;
		send_message_ser(ptr,&msg2);
		}
		else
		{
		msg2.name[0] = 3;
		send_message_ser(ptr,&msg2);
		}
	}

	return 0;
}


int send_message_ser(parameter *ptr,message *msg)
{
	if(send(ptr->newfd,msg,sizeof(message),0) < 0)
	{
		ERRMSG("send");
		return -1;
	}
	return 0;
}

int make_message(message *msg,char **presult)
{
	if(presult[8] != NULL)
	{
		strcpy(msg->name,presult[8]);
	}
	if(presult[9] != NULL)
	{
		strcpy(msg->phone,presult[9]);
	}
	if(presult[10] != NULL)
	{
		strcpy(msg->passwd,presult[10]);
	}
	if(presult[11] != NULL)
	{
		strcpy(msg->addr,presult[11]);
	}
	if(presult[12] != NULL)
	{
		strcpy(msg->salary,presult[12]);
	}
	if(presult[13] != NULL)
	{
		strcpy(msg->age,presult[13]);
	}
	if(presult[14] != NULL)
	{
		strcpy(msg->type,presult[14]);
	}
	if(presult[15] != NULL)
	{
		strcpy(msg->power,presult[15]);
	}
	return 0;
}

int update_message_ser(message *newmsg,message *msg)
{
	if(newmsg->name[0] != 0)
	{
		strcpy(msg->name,newmsg->name);
	}
	if(newmsg->phone[0] != 0)
	{
		strcpy(msg->phone,newmsg->phone);
	}
	if(newmsg->passwd[0] != 0)
	{
		strcpy(msg->passwd,newmsg->passwd);
	}
	if(newmsg->addr[0] != 0)
	{
		strcpy(msg->addr,newmsg->addr);
	}
	if(newmsg->salary[0] != 0)
	{
		strcpy(msg->salary,newmsg->salary);
	}
	if(newmsg->age[0] != 0)
	{
		strcpy(msg->age,newmsg->age);
	}
	if(newmsg->type[0] != 0)
	{
		strcpy(msg->type,newmsg->type);
	}
	if(newmsg->power[0] != 0)
	{
		strcpy(msg->power,newmsg->power);
	}
	return 0;
}

int change_message_ser(message *msg,parameter *ptr,message *oldmsg)
{
	char buf[128] ="";
	char *errmsg;
	printf("msg%s\n",oldmsg->phone);
	sprintf(buf,"update staff set name=\"%s\" where phone=\"%s\";",msg->name,oldmsg->phone);
	if(sqlite3_exec(ptr->efpd,buf,NULL,NULL,&errmsg) != SQLITE_OK)
	{
		printf("__%d__%s\n",__LINE__,errmsg);
		return 3;
	}
	sprintf(buf,"update staff set phone=\"%s\" where phone=\"%s\";",msg->phone,oldmsg->phone);
	if(sqlite3_exec(ptr->efpd,buf,NULL,NULL,&errmsg) != SQLITE_OK)
	{
		printf("__%d__%s\n",__LINE__,errmsg);
		return 3;
	}
	sprintf(buf,"update staff set passwd=\"%s\" where phone=\"%s\";",msg->passwd,oldmsg->phone);
	if(sqlite3_exec(ptr->efpd,buf,NULL,NULL,&errmsg) != SQLITE_OK)
	{
		printf("__%d__%s\n",__LINE__,errmsg);
		return 3;
	}
	sprintf(buf,"update staff set salary=\"%s\" where phone=\"%s\";",msg->salary,oldmsg->phone);
	if(sqlite3_exec(ptr->efpd,buf,NULL,NULL,&errmsg) != SQLITE_OK)
	{
		printf("__%d__%s\n",__LINE__,errmsg);
		return 3;
	}
	sprintf(buf,"update staff set age=\"%s\" where phone=\"%s\";",msg->age,oldmsg->phone);
	if(sqlite3_exec(ptr->efpd,buf,NULL,NULL,&errmsg) != SQLITE_OK)
	{
		printf("__%d__%s\n",__LINE__,errmsg);
		return 3;
	}
	sprintf(buf,"update staff set type=\"%s\" where phone=\"%s\";",msg->type,oldmsg->phone);
	if(sqlite3_exec(ptr->efpd,buf,NULL,NULL,&errmsg) != SQLITE_OK)
	{
		printf("__%d__%s\n",__LINE__,errmsg);
		return 3;
	}
	sprintf(buf,"update staff set power=\"%s\" where phone=\"%s\";",msg->power,oldmsg->phone);
	if(sqlite3_exec(ptr->efpd,buf,NULL,NULL,&errmsg) != SQLITE_OK)
	{
		printf("__%d__%s\n",__LINE__,errmsg);
		return 3;
	}
	return 0;
}
