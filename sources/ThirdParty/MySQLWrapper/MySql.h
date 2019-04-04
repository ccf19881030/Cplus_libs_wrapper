//
// MySql.h
//
// MySql imp.
//
// Copyright (c) 2009, Renato Tegon Forti - re.tf@acm.org
//
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Exception.h"
#include "ResultSet.h"
#include "DataBase.h"
#include <mysql.h> 


class MySql
	/// MySql data base class
{
	friend class DataBase<MySql>;

public:

	MySql()
	{
	}

	virtual ~MySql()
	{
	}
	
	void connect(const std::string& server, const std::string& user, const std::string& password, const std::string& database)
	{		
		_connectionHandlerPtr = mysql_init(NULL); 
				
		if (NULL == mysql_real_connect(_connectionHandlerPtr, server.c_str(), user.c_str(), password.c_str(), database.c_str(), 0, NULL, 0)) 
		{ 
			throw DataBaseError("Failed to connect to database: Error: " + std::string(mysql_error(_connectionHandlerPtr)));
		} 
	}

	void execute(const std::string& sql)
	{
		std::cout << sql << std::endl;

		if(!(mysql_query(_connectionHandlerPtr, sql.c_str()) == 0) )
		{ 
			throw DataBaseError("Failed to execute sql: Error: " + std::string(mysql_error(_connectionHandlerPtr)));
		}
	}

	void populate(ResultSet& rs)
	{
		MYSQL_RES *result = NULL; // result of querying for all rows in table 

		// You must call mysql_store_result() or mysql_use_result() 
		// for every query that successfully retrieves data (SELECT, SHOW, DESCRIBE, EXPLAIN). 

		result = mysql_use_result(_connectionHandlerPtr); 

		MYSQL_ROW row;
		unsigned int num_fields;
		unsigned int i;

		num_fields = mysql_num_fields(result);

		// get rows
		while ((row = mysql_fetch_row(result)))
		{
			//unsigned long *lengths;
			//lengths = mysql_fetch_lengths(result);

			 std::vector<std::string> myRow;

			// get fields od row
			for(i = 0; i < num_fields; i++)
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

			rs.addRow(myRow);
		}

		mysql_free_result(result); 
	}

protected:

	void close(void)
	{
		mysql_close(_connectionHandlerPtr); 
	}
	
private:
	
	MYSQL* _connectionHandlerPtr;
	// This structure represents a handle to one database connection. 
	// It is used for almost all MySQL functions. 
	// You should not try to make a copy of a MYSQL structure. 
	// There is no guarantee that such a copy will be usable. 

}; // MySql
