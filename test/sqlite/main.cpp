// test/sqlite/main.h
// Robin.Rowe@Cinepaint.org
// 2018/9/19

#include <sqlite/Db.h>
#include <sqlite/Statement.h>
#include <string>

enum
{	SUCCESS,
	BAD_OPEN,
	BAD_EXEC,
	BAD_EXEC2,
	BAD_EXEC3,
	BAD_BIND,
	BAD_BIND2,
	BAD_STATEMENT
};
	
int ExitMsg(const char* status,const char* msg,int code)
{	printf("%s: %s\n",status,msg); 
	return code;
}

#define exit_return(status,x) return ExitMsg(status,#x,x)

int main(int argc, char* argv[])
{	sqlite::Db db;
	const char* dbName = "test.db";
	db.DropFile(dbName);
	if(!db.Open(dbName))
	{	exit_return(dbName,BAD_OPEN);
	}
	fprintf(stderr, "Opened database successfully\n");
	const char* sql = "CREATE TABLE COMPANY("
		 "ID INT PRIMARY KEY     NOT NULL,"
		 "NAME           TEXT    NOT NULL,"
		 "AGE            INT     NOT NULL,"
		 "ADDRESS        CHAR(50),"
		 "SALARY         REAL );";
	if(!db.Exec(sql,sqlite::Db::PrintCb,&db))
	{	exit_return(db.GetErrorMsg(),BAD_EXEC);
	}
	else
	{	puts("Table created successfully");
	}
	sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
		 "VALUES (1, 'Paul', 32, 'California', 20000.00 ); "
		 "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
		 "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "
		 "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
		 "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );"
		 "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
		 "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";
	if(!db.Exec(sql,sqlite::Db::PrintCb,&db))
	{	exit_return(db.GetErrorMsg(),BAD_EXEC2);
	}
	else
	{	puts("Table inserted successfully");
	}
	sql = "SELECT * from COMPANY";
	if(!db.Exec(sql,sqlite::Db::PrintCb,&db))
	{	exit_return(db.GetErrorMsg(),BAD_EXEC3);
	}
	else
	{	puts("Table queried successfully");
	}
	const char* query = "SELECT id, name FROM COMPANY WHERE id = ?1;";
	sqlite::Statement s(db,query);
	if(!s)
	{	exit_return(db.GetErrorMsg(),BAD_STATEMENT);
	}
	if(!s.Bind(2))
	{	exit_return(db.GetErrorMsg(),BAD_BIND);
	}
	while(s.Step())
	{	int id = 0;
		const char* name = 0;
		s.Get(id,0);
		s.Get(name,1);
		printf("bind = %i,%s\n",id,name);
	}
	exit_return("Done",SUCCESS);
}
//$g++ test.c -l sqlite3

