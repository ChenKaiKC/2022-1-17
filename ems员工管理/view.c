#include "inc.h"

int view_1(int *sockfd)
{
	char choice;
	while(1)
	{
		printf("------------------------\n");
		printf("---------1.注册---------\n");
		printf("---------2.登录---------\n");
		printf("---------0.退出---------\n");
		printf("------------------------\n");
		printf("请选择>>");
		fflush(stdout);

		choice = getchar();
		getchar();
		{
			switch(choice)
			{
			case '1':
				printf("------------------------\n");
				printf("-------1.员工注册-------\n");
				printf("------2.管理员注册------\n");
				printf("---------0.退出---------\n");
				printf("------------------------\n");
				printf("请选择>>");
				fflush(stdout);
				choice = getchar();
				getchar();
				if(choice == '1')
				{
					//员工注册
					//printf("员工注册\n");
					do_register_staff_cil(sockfd,0);
				}
				else if(choice == '2')
				{
					//管理员注册
					//printf("管理员注册\n");
					do_register_staff_cil(sockfd,1);
				}
				else if(choice == '0')
				{
					continue;
				}
				else
				{
					printf("输入错误按任意键继续\n");
					while(getchar() != '\n');
				}
				break;
			case '2':
				printf("------------------------\n");
				printf("-------1.员工登录-------\n");
				printf("------2.管理员登录------\n");
				printf("---------0.退出---------\n");
				printf("------------------------\n");
				printf("请选择>>");
				fflush(stdout);
				choice = getchar();
				getchar();
				if(choice == '1')
				{
					//员工登录
					//printf("员工登录\n");
					do_login_staff_cil(sockfd,0);
				}
				else if(choice == '2')
				{
					//管理员登录
					//printf("管理员登录\n");
					do_login_staff_cil(sockfd,1);
				}
				else if(choice == '0')
				{
					continue;
				}
				else
				{
					printf("输入错误按任意键继续\n");
					while(getchar() != '\n');
				}
				break;
			case '0':
				return 0;
			default:
				printf("输入错误按任意键继续\n");
				while(getchar() != '\n');
				break;
			}
		}
	}
}

int view_staff(int *sockfd,char *phone)
{
	char choice;
	char buf[64] ="";
	message msg;
	while(1)
	{
		printf("------------------------\n");
		printf("-----1.查看个人信息-----\n");
		printf("-----2.修改个人信息-----\n");
		printf("-------0.退出登录-------\n");
		printf("------------------------\n");
		printf("请选择>>");
		fflush(stdout);
		choice = getchar();
		getchar();
		switch(choice)
		{
		case '1':
			strcpy(buf,CHECK_SELF);
			strcat(buf,phone);
			if(send(*sockfd,buf,sizeof(buf),0) < 0)
			{
				ERRMSG("send");
				return -1;
			}
			memset(&msg,0,sizeof(msg));
			if(recv(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("recv");
				return -1;
			}
			if(msg.name[0] == '7')
			{
				printf("%s\n",msg.name+1);
				continue;
			}
			dispaly_message(&msg);
			break;
		case '2':
			view_change_staff(sockfd,phone);
			break;
		case '0':
			buf[0] = '0';
			if(send(*sockfd,buf,sizeof(buf),0) < 0)
			{
				ERRMSG("send");
				return -1;
			}
			return 0;
			break;
		}
	}
	return 0;
}

int view_admin(int *sockfd,char *phone)
{
	int i;
	char choice;
	char buf[64] ="";
	message msg;
	while(1)
	{
		printf("------------------------\n");
		printf("-----1.查看个人信息-----\n");
		printf("-----2.修改个人信息-----\n");
		printf("-----3.查找他人信息-----\n");
		printf("-----4.修改他人信息-----\n");
		printf("-------5.增加员工-------\n");
		printf("-------6.删除员工-------\n");
		printf("-------0.退出登录-------\n");
		printf("------------------------\n");
		printf("请选择>>");
		fflush(stdout);
		choice = getchar();
		getchar();
		switch(choice)
		{
		case '1':
			strcpy(buf,CHECK_SELF);
			strcat(buf,phone);
			if(send(*sockfd,buf,sizeof(buf),0) < 0)
			{
				ERRMSG("send");
				return -1;
			}
			memset(&msg,0,sizeof(msg));
			if(recv(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("recv");
				return -1;
			}
			if(msg.name[0] == '7')
			{
				printf("%s\n",msg.name+1);
				continue;
			}
			dispaly_message(&msg);
			break;
		case '2':
			view_change_staff(sockfd,phone);
			break;
		case '3':
			while(1)
			{
				printf("新电话号码:");
				fflush(stdout);
				scanf("%s",msg.phone);
				getchar();
				for(i=0;i<13;)
				{
					if(msg.phone[i] >= '0'&&msg.phone[i] <= '9'&&msg.phone[13] == 0)
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

			strcpy(buf,CHECK_OTHER);
			strcat(buf,msg.phone);
			if(send(*sockfd,buf,sizeof(buf),0) < 0)
			{
				ERRMSG("send");
				return -1;
			}
			memset(&msg,0,sizeof(msg));
			if(recv(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("recv");
				return -1;
			}
			if(msg.name[0] == '7')
			{
				printf("%s\n",msg.name+1);
				continue;
			}
			dispaly_message_admin(&msg);
			break;
		case '4':
		while(1)
			{
				printf("请输入电话号码:");
				fflush(stdout);
				scanf("%s",msg.phone);
				getchar();
				for(i=0;i<13;)
				{
					if(msg.phone[i] >= '0'&&msg.phone[i] <= '9'&&msg.phone[13] == 0)
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
			view_change_admin(sockfd,msg.phone);
			break;

		case '5':
			do_register_staff_cil(sockfd,0);
			break;
		case '6':
	while(1)
			{
				printf("要删除员工的电话号码:");
				fflush(stdout);
				scanf("%s",msg.phone);
				getchar();
				for(i=0;i<13;)
				{
					if(msg.phone[i] >= '0'&&msg.phone[i] <= '9'&&msg.phone[13] == 0)
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
			strcpy(buf,DELATE_OTHER);
			strcat(buf,msg.phone);
			if(send(*sockfd,buf,sizeof(buf),0) < 0)
			{
				ERRMSG("send");
				return -1;
			}
			memset(&msg,0,sizeof(msg));
			if(recv(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("recv");
				return -1;
			}
			if(msg.name[0] == 1)
			{
				printf("删除成功\n");
			}
			else
			{
				printf("删除失败\n");
			}
			break;
		case '0':
			buf[0] = '0';
			if(send(*sockfd,buf,sizeof(buf),0) < 0)
			{
				ERRMSG("send");
				return -1;
			}
			return 0;
			break;
		}
	}

	return 0;
}

int dispaly_message(message *msg)
{
	printf("姓名\t电话\t\t地址\t\t薪资\t年龄\t身份\n");
	if(msg->power[0] == '1')
	printf("%s\t%s\t%s\t%s\t%s\t%s\n",msg->name,msg->phone,msg->addr,msg->salary,msg->age,"管理员");
	if(msg->power[0] == '0')
	printf("%s\t%s\t%s\t%s\t%s\t%s\n",msg->name,msg->phone,msg->addr,msg->salary,msg->age,"员工");
	return 0;
}

int dispaly_message_admin(message *msg)
{
	printf("姓名\t电话\t\t地址\t\t薪资\t年龄\t身份\t状态\n");
	if(msg->power[0] == '1')
	{
		if(msg->type[0] == '1')
		printf("%s\t%s\t%s\t%s\t%s\t%s\t%s\n",msg->name,msg->phone,msg->addr,msg->salary,msg->age,"管理员","在线");
		if(msg->type[0] == '0')
		printf("%s\t%s\t%s\t%s\t%s\t%s\t%s\n",msg->name,msg->phone,msg->addr,msg->salary,msg->age,"管理员","离线");
	}
	if(msg->power[0] == '0')
	{
		if(msg->type[0] == '1')
		printf("%s\t%s\t%s\t%s\t%s\t%s\t%s\n",msg->name,msg->phone,msg->addr,msg->salary,msg->age,"员工","在线");
		if(msg->type[0] == '0')
		printf("%s\t%s\t%s\t%s\t%s\t%s\t%s\n",msg->name,msg->phone,msg->addr,msg->salary,msg->age,"员工","离线");
	}
	return 0;
}

int view_change_admin(int *sockfd,char *phone)
{
	char choice;
	int i;
	message msg;
	strcpy(msg.phone,CHANGE_OTHER);
	strcat(msg.phone,phone);
	if(send(*sockfd,msg.phone,64,0) < 0)
	{
		ERRMSG("send");
		return -1;
	}
	memset(&msg,0,sizeof(msg));	
	while(1)
	{
		printf("------------------------\n");
		printf("-------1.修改姓名-------\n");
		printf("-------2.修改手机-------\n");
		printf("-------3.修改密码-------\n");
		printf("-------4.修改地址-------\n");
		printf("-------5.修改薪资-------\n");
		printf("-------6.修改年龄-------\n");
		printf("-------7.修改权限-------\n");
		printf("-------0.退出修改-------\n");
		printf("------------------------\n");
		printf("请选择>>");
		fflush(stdout);
		choice = getchar();
		getchar();
		switch(choice)
		{
		case '1':
			printf("新名字是>>");
			scanf("%s",msg.name);
			getchar();
			if(send(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("send");
				return -1;
			}
			
			memset(&msg,0,sizeof(msg));
			if(recv(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("recv");
				return -1;
			}
			if(msg.name[0] == '7')
			{
				printf("%s\n",msg.name+1);
			}
			else if(msg.name[0] = 1)
			{
				printf("修改成功\n");
			}
			else
			{
				printf("修改失败\n");
			}
			memset(&msg,0,sizeof(msg));
			break;
		case '2':
			while(1)
			{
				printf("请输入电话号码:");
				fflush(stdout);
				scanf("%s",msg.phone);
				getchar();
				for(i=0;i<13;)
				{
					if(msg.phone[i] >= '0'&&msg.phone[i] <= '9'&&msg.phone[13] == 0)
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
			if(send(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("send");
				return -1;
			}
			memset(&msg,0,sizeof(msg));	
			if(recv(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("recv");
				return -1;
			}
			if(msg.name[0] == '7')
			{
				printf("%s\n",msg.name+1);
			}
			else if(msg.name[0] = 1)
			{
				printf("修改成功\n");
			}
			else
			{
				printf("修改失败\n");
			}
			memset(&msg,0,sizeof(msg));
			break;
		case '3':
			printf("新密码是>>");
			scanf("%s",msg.passwd);
			getchar();
			if(send(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("send");
				return -1;
			}
			memset(&msg,0,sizeof(msg));
			if(recv(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("recv");
				return -1;
			}
			if(msg.name[0] == '7')
			{
				printf("%s\n",msg.name+1);
			}
			else if(msg.name[0] = 1)
			{
				printf("修改成功\n");
			}
			else
			{
				printf("修改失败\n");
			}
			memset(&msg,0,sizeof(msg));
			break;
		case '4':
			printf("新地址是>>");
			scanf("%s",msg.addr);
			getchar();
			if(send(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("send");
				return -1;
			}
			memset(&msg,0,sizeof(msg));
			if(recv(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("recv");
				return -1;
			}
			if(msg.name[0] == '7')
			{
				printf("%s\n",msg.name+1);
			}
			else if(msg.name[0] = 1)
			{
				printf("修改成功\n");
			}
			else
			{
				printf("修改失败\n");
			}
			memset(&msg,0,sizeof(msg));
			break;
		case '5':
			printf("新薪资是>>");
			scanf("%s",msg.salary);
			getchar();
			if(send(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("send");
				return -1;
			}
			memset(&msg,0,sizeof(msg));
			if(recv(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("recv");
				return -1;
			}
			if(msg.name[0] == '7')
			{
				printf("%s\n",msg.name+1);
			}
			else if(msg.name[0] = 1)
			{
				printf("修改成功\n");
			}
			else
			{
				printf("修改失败\n");
			}
			memset(&msg,0,sizeof(msg));
			break;
		case '6':	
			printf("新年龄是>>");
			scanf("%s",msg.age);
			getchar();
			if(send(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("send");
				return -1;
			}
			memset(&msg,0,sizeof(msg));
			if(recv(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("recv");
				return -1;
			}
			if(msg.name[0] == '7')
			{
				printf("%s\n",msg.name+1);
			}
			else if(msg.name[0] = 1)
			{
				printf("修改成功\n");
			}
			else
			{
				printf("修改失败\n");
			}
			memset(&msg,0,sizeof(msg));
			break;
		case '7':
			printf("新权限是(输入1改为管理员 输入0改为员工)>>");
			scanf("%s",msg.power);
			getchar();
			if(send(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("send");
				return -1;
			}
			memset(&msg,0,sizeof(msg));
			if(recv(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("recv");
				return -1;
			}
			if(msg.name[0] == '7')
			{
				printf("%s\n",msg.name+1);
			}
			else if(msg.name[0] = 1)
			{
				printf("修改成功\n");
			}
			else
			{
				printf("修改失败\n");
			}
			memset(&msg,0,sizeof(msg));
			break;
		case '0':
			return 0;
			break;
		default:
			printf("输入错误\n");
			break;
		}
	}
	return 0;
}

int view_change_staff(int *sockfd,char *phone)
{
	char choice;
	int i;
	message msg;
	strcpy(msg.phone,CHANGE_SELF);
	strcat(msg.phone,phone);
	if(send(*sockfd,msg.phone,64,0) < 0)
	{
		ERRMSG("send");
		return -1;
	}
	memset(&msg,0,sizeof(msg));	
	while(1)
	{
		printf("------------------------\n");
		printf("-------1.修改姓名-------\n");
		printf("-------2.修改手机-------\n");
		printf("-------3.修改密码-------\n");
		printf("-------4.修改地址-------\n");
		printf("-------0.退出修改-------\n");
		printf("------------------------\n");
		printf("请选择>>");
		fflush(stdout);
		choice = getchar();
		getchar();
		switch(choice)
		{
		case '1':
			printf("新名字是>>");
			scanf("%s",msg.name);
			getchar();
			if(send(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("send");
				return -1;
			}
			
			memset(&msg,0,sizeof(msg));
			if(recv(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("recv");
				return -1;
			}
			if(msg.name[0] == '7')
			{
				printf("%s\n",msg.name+1);
			}
			else if(msg.name[0] = 1)
			{
				printf("修改成功\n");
			}
			else
			{
				printf("修改失败\n");
			}
			memset(&msg,0,sizeof(msg));
			break;
		case '2':
			while(1)
			{
				printf("新电话号码:");
				fflush(stdout);
				scanf("%s",msg.phone);
				getchar();
				for(i=0;i<13;)
				{
					if(msg.phone[i] >= '0'&&msg.phone[i] <= '9'&&msg.phone[13] == 0)
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
			if(send(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("send");
				return -1;
			}
			memset(&msg,0,sizeof(msg));	
			if(recv(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("recv");
				return -1;
			}
			if(msg.name[0] == '7')
			{
				printf("%s\n",msg.name+1);
			}
			else if(msg.name[0] = 1)
			{
				printf("修改成功\n");
			}
			else
			{
				printf("修改失败\n");
			}
			memset(&msg,0,sizeof(msg));
			break;
		case '3':
			printf("新密码是>>");
			scanf("%s",msg.passwd);
			getchar();
			if(send(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("send");
				return -1;
			}
			memset(&msg,0,sizeof(msg));
			if(recv(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("recv");
				return -1;
			}
			if(msg.name[0] == '7')
			{
				printf("%s\n",msg.name+1);
			}
			else if(msg.name[0] = 1)
			{
				printf("修改成功\n");
			}
			else
			{
				printf("修改失败\n");
			}
			memset(&msg,0,sizeof(msg));
			break;
		case '4':
			printf("新地址是>>");
			scanf("%s",msg.addr);
			getchar();
			if(send(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("send");
				return -1;
			}
			memset(&msg,0,sizeof(msg));
			if(recv(*sockfd,&msg,sizeof(msg),0) < 0)
			{
				ERRMSG("recv");
				return -1;
			}
			if(msg.name[0] == '7')
			{
				printf("%s\n",msg.name+1);
			}
			else if(msg.name[0] = 1)
			{
				printf("修改成功\n");
			}
			else
			{
				printf("修改失败\n");
			}
			memset(&msg,0,sizeof(msg));
			break;
		case '0':
			return 0;
			break;
		default:
			printf("输入错误\n");
			break;
		}
	}
	return 0;
}
