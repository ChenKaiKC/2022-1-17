#include "inc.h"

int main(int argc, const char *argv[])
{
	sqlite3 *pd;
	char **presult;
	char *errmsg;
	int line = 0;
	int row = 0;
	char buf[128] ="";
	import_form(&pd);
	sprintf(buf,"select * from staff where phone=1231231231231;");
	if(sqlite3_get_table(pd,buf,&presult,&line,&row,&errmsg) != SQLITE_OK)
	{
		printf("__%d__%s\n",__LINE__,errmsg);
		return -1;
	}
	for(int i = 0;i<16;i++)
	{
		if(presult[i] != NULL)
		printf("%s\n",presult[i]);
	}
	return 0;
}
