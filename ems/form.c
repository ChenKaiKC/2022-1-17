#include "inc.h"

int import_form(sqlite3 **efpd)
{
	char filename[50] = "form.db";
	char buf[128] = "";
	char *errmsg;
	
	//打开或创建并打开员工数据库
	if(sqlite3_open(filename,efpd) != SQLITE_OK)
	{
		printf("__%d__%s\n",__LINE__,sqlite3_errmsg(*efpd));
		return -1;
	}
	printf("打开员工数据库成功\n");
	
	//创建或者打开员工表格
	sprintf(buf,"create table if not exists staff(name char,phone char primary key,passwd char,addr char,salary char,age char,type char,power char);");
	if(sqlite3_exec(*efpd,buf,NULL,NULL,&errmsg) != SQLITE_OK)
	{
		printf("创建员工表格失败\n");
		printf("__%d__%s\n",__LINE__,errmsg);
		return -1;
	}
	bzero(buf,sizeof(buf));

	//初始化员工表格
	sprintf(buf,"update staff set type=0 where type=\"type\";");
	if(sqlite3_exec(*efpd,buf,NULL,NULL,&errmsg) != SQLITE_OK)
	{
		printf("更新员工表格失败\n");
		printf("__%d__%s\n",__LINE__,errmsg);
		return -1;
	}
	printf("员工数据库初始化成功\n");
	return 0;
}
