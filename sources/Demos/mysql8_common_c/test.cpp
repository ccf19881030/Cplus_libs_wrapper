#include <iostream>
#include <string>
#include <vector>

#include "mysql.h"

#pragma comment(lib, "libmysql.lib")

using namespace std;

int main(int argc, char *argv[])
{
   
	MYSQL* mysqlConnHandle;

	mysqlConnHandle = mysql_init(NULL);
	mysql_options(mysqlConnHandle, MYSQL_SET_CHARSET_NAME, "gbk");//连接选项：添加GBK字符集支持

	if (NULL == mysql_real_connect(mysqlConnHandle, "127.0.0.1", "username", "password", 0, NULL, 0))
	{
		std::cout << "Failed to connect to database: Error: " << mysql_error(mysqlConnHandle);
	}

	string sql = "SELECT * FROM monitordata";
	if (!(mysql_query(mysqlConnHandle, sql.c_str()) == 0))
	{
		cout << "Failed to execute sql:" << sql << ",Error: " << mysql_error(mysqlConnHandle);
	}

	MYSQL_RES *result = NULL; // result of querying for all rows in table 

	// You must call mysql_store_result() or mysql_use_result() 
	// for every query that successfully retrieves data (SELECT, SHOW, DESCRIBE, EXPLAIN). 

	result = mysql_use_result(mysqlConnHandle);

	MYSQL_ROW row;
	unsigned int num_fields;
	unsigned int i;
	unsigned int j = 0;

	num_fields = mysql_num_fields(result);

	// get rows
	while ((row = mysql_fetch_row(result)))
	{
		//unsigned long *lengths;
		//lengths = mysql_fetch_lengths(result);

		std::vector<std::string> myRow;

		// get fields od row
		for (i = 0; i < num_fields; i++)
		{
			if (row[i] == NULL)
			{
				myRow.push_back("NULL");
			}
			else
			{
				myRow.push_back(row[i]);
			}
		}

		//print the query select result
		cout << "the " << ++j << "row data：";
		for (auto it = myRow.begin(); it != myRow.end(); ++it)
		{
			cout << (*it) << "|";
		}
		cout << std::endl;
	}

	mysql_free_result(result);

	mysql_close(mysqlConnHandle);

	return 0;
}
