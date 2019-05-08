#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

#include <fstream>

using boost::asio::ip::tcp;
using namespace std;

/****************************************************************************************************
Exercise
Develop a client and a server which can transfer a file from one computer to another. 

When the server is started, it should display a list of IP addresses of all local interfaces and wait 
for the client to connect. 

When the client is started, an IP address from the server and the name of
a local file should be passed as command line options. 
The client should transfer the file to the server 
which saves it to the current working directory. During transmission the client should display some sort 
of progress indicator so that the user knows that the transmission is ongoing. 
Implement the client and server with callbacks.

*****************************************************************************************************/

typedef struct  FileInfo
{
	FileInfo(std::size_t fileLen, std::string fileContent)
	{
		fileLength_ = fileLen;
		fileContent_ = fileContent;
	}
	std::size_t  fileLength_;				// 文件长度
	//std::string filePath_;				// 文件路径
	std::string fileContent_;				// 文件内容
	//std::string fileExtent_;				// 文件扩展名
}FileInfo;

class File_Client
{
public:
	File_Client(boost::asio::io_context& io_context,
		const tcp::resolver::results_type& endpoints,int fileLen, std::string fileContent)
		: io_context_(io_context),
		socket_(io_context),
		fileInfo_(fileLen, fileContent)
	{
		do_connect(endpoints);
	}

	void close()
	{
		boost::asio::post(io_context_, [this]() { socket_.close(); });
	}

private:
	void do_connect(const tcp::resolver::results_type& endpoints)
	{
		boost::asio::async_connect(socket_, endpoints,
			[this](boost::system::error_code ec, tcp::endpoint)
		{
			if (!ec)
			{
				// 发送文件大小到服务端
				do_write_fileLength();
			}
		});
	}

	void do_write_fileLength()
	{
		static char strFileLen[5] = "";
		std::sprintf(strFileLen, "%d", fileInfo_.fileLength_);
	
		boost::asio::async_write(socket_,
			boost::asio::buffer(strFileLen, 5),
			[this](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				std::cout.write(strFileLen, 5);
				std::cout << "字节" << "\n";

				// 发送文件内容
				// 每次发送1k = 1024 bytes的数据到服务器端
				int nCount = fileInfo_.fileLength_ / PER_SEND_LENGTH;
				
				int i = 0;
				while (i < nCount)
				{
					std::string per_data;
					per_data.assign(fileInfo_.fileContent_.begin() + current_send_Len,
						fileInfo_.fileContent_.begin() + current_send_Len + PER_SEND_LENGTH);

					current_send_Len += PER_SEND_LENGTH;

					do_send_file(per_data);

					i++;
				}

				// 如果文件长度小于1024字节，则一次性发送所有数据
				// 或者发送最后不足1024字节的数据
				std::string last_data;
				last_data.assign(fileInfo_.fileContent_.begin() + current_send_Len,
					fileInfo_.fileContent_.end());
				do_send_file(last_data);
			}
			else
			{
				socket_.close();
			}
		});
	}

	void do_send_file(const std::string &buffer)
	{
		boost::asio::async_write(socket_,
			boost::asio::buffer(buffer.data(), buffer.size()),
			[this](boost::system::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				static int i = 0;
				std::cout << "第" << ++i << "次发送，" << "发送的字节数为："
					<< length << std::endl;
				
				int current_progress = (i-1) * PER_SEND_LENGTH + length;
				std::cout << "文件当前发送进度 ：" 
						<< (current_progress * 1.000 / fileInfo_.fileLength_)  * 100.00
						<< "%"  << std::endl;
			}
			else
			{
				socket_.close();
			}
		});
	}

public:
	FileInfo fileInfo_;
	static int current_send_Len;  
	enum { PER_SEND_LENGTH = 1024 };
private:
	boost::asio::io_context& io_context_;
	tcp::socket socket_;
};


int File_Client::current_send_Len = 0;

int main(int argc, char* argv[])
{

	try
	{
		if (argc != 4)
		{
			std::cerr << "Usage: chat_client <host> <port> <filePath>\n";
			return 1;
		}

		boost::asio::io_context io_context;

		tcp::resolver resolver(io_context);
		auto endpoints = resolver.resolve(argv[1], argv[2]);
		
		// 从命令行参数中获取文件路径
		std::string filePath = argv[3];


		std::ifstream is(filePath.c_str(), std::ifstream::binary);
		std::size_t length = 0;
		if (!is)
		{
			std::cout << "读取文件:" << filePath.c_str() << "失败!" << std::endl;
		}
		

		// 获取文件长度
		is.seekg(0, is.end);
		length = (std::size_t)is.tellg();
		is.seekg(0, is.beg);

		char * buffer = new char[length];

		std::cout << "Reading " << length << " characters... ";
		// read data as a block:
		is.read(buffer, length);

		if (is)
			std::cout << "all characters read successfully." << std::endl;
		else
			std::cout << "error: only " << is.gcount() << " could be read" << std::endl;
		is.close();

		// ...buffer contains the entire file...

		File_Client client(io_context, endpoints, length, buffer);

		delete[] buffer;


		std::thread t([&io_context]() { io_context.run(); });

		client.close();

		t.join();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;

	std::system("pause");

	return 0;
}
