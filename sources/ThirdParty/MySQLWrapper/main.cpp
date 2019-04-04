//
//main.cpp
//
// This sample demonstrates the MySql/PostgreSql class.
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


// Std
#include <iostream>
#include <winsock2.h> 
#include "db/MySql.h"
//#include "db/PostgreSql.h"


int main(int argc, char** argv)
{
	try
	{	
		ResultSet rs1, rs2;
		std::vector<std::string> row;
		
		DataBase<MySql /* or PostgreSql  */> dataBase("205.178.146.20", "dot", "Dot.Fidelity1", "dot");

		// table one
		// 

		dataBase << "DROP TABLE if  exists  tblTest1";
		dataBase <<	"CREATE TABLE if not exists tblTest1(test char(15) NOT NULL, testInt INT NULL, Constraint PK Primary Key(test))";
		dataBase << "DELETE FROM tblTest1";

		dataBase << "INSERT INTO tblTest1(test, testInt) VALUES('00', 1)" ;
		dataBase << "INSERT INTO tblTest1(test) VALUES('01')" ;
		dataBase << "INSERT INTO tblTest1(test) VALUES('02')" ;
		dataBase << "INSERT INTO tblTest1(test) VALUES('03')" ;


		// table two
		// 

		dataBase << "DROP TABLE tblTest2";
		dataBase <<	"CREATE TABLE if not exists tblTest2(test char(15) NOT NULL, dt DATE NULL, Constraint PK Primary Key(test))";
		dataBase << "DELETE FROM tblTest2";

		dataBase << "INSERT INTO tblTest2(test, dt) VALUES('01', '1979/11/14')" ;
		dataBase << "INSERT INTO tblTest2(test) VALUES('02')" ;
		dataBase << "INSERT INTO tblTest2(test) VALUES('03')" ;
		dataBase << "INSERT INTO tblTest2(test) VALUES('04')" ;

		dataBase << "SELECT * FROM tblTest1, tblTest2 WHERE tblTest1.test = tblTest2.test", rs1;
		dataBase << "SELECT * FROM tblTest2 WHERE test = '01'", rs2;

		std::string value;

		while(rs1.fetch(0, value))
		{
			std::cout << value << std::endl;
		}

		while(rs2.fetch(1, value))
		{
			std::cout << value << std::endl;
		}

		while(rs1.fetch(row))
		{
			for (size_t i = 0; i < row.size(); i++)
			{
				std::cout << row[i] <<  " | ";
			}
		}

		while(rs2.fetch(row))
		{
			for (size_t i = 0; i < row.size(); i++)
			{
				std::cout << row[i] << " | ";
			}
		}

		std::cout << rs1.get(0)[0] << " | " << rs1.get(0)[1] << std::endl;
		std::cout << rs2.get(0, 1) << std::endl;
	}
	catch (const DataBaseError& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}
