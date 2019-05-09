#include <boost/crc.hpp>
#include <iostream>

using namespace boost;
using namespace std;

void crc32_Demo1()
{
	crc_32_type crc32;						      	      // 一个crc对象

	cout << hex;								                // 置输出流为hex格式输入
	cout << crc32.checksum() << endl;			      // 无数据输入时也可以取crc值

	crc32.process_byte('a');					          // 计算一个字节
	cout << crc32.checksum() << endl;

	crc32.process_bytes("1234567890", 10);
	cout << crc32.checksum() << endl;

	char szChar[] = "1234567890";			        	// 一个字符数组
	crc32.reset();								              // 复位crc处理机

	crc32.process_block(szChar, szChar + 10);	  // 使用区间的形式输入数据
	cout << crc32.checksum() << endl;
}

/********************************************************************************
crc_opotimal还重载了括号操作符operator()，它有两种用法：
不带参数的形式直接返回CRC值，相当于调用checksum()，可以很方便地获取返回值；
带参数的形式接受一个字节，相当于调用process_byte()，因此可以把crc_optimal对象当作函数对象
传递给标准算法。
********************************************************************************/
void crc32_demo2()
{
	crc_32_type crc32;							              // 一个crc对象

	cout << hex;
	crc32.process_bytes("1234567890", 10);		    // 处理10个字节
	cout << crc32() << endl;					            // 使用operator()获得crc值

	std::string str = "1234567890";
	crc32.reset();								                // 复位crc处理机
	//把crc对象作为函数对象传递给for_each算法处理字符串
	cout << std::for_each(str.begin(), str.end(), crc32)() << endl;
	cout << crc32() << endl;
}

int main(int argc, char *argv[])
{
	
	cout << "-----------------------crc32_Demo1() test----------------------------------" << endl;
	crc32_Demo1();
	cout << "-----------------------crc32_Demo2() test----------------------------------" << endl;
	crc32_demo2();

	std::system("pause");

	return 0;
}
