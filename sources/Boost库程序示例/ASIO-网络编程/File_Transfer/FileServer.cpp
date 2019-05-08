/***************************************************************************************************************
Exercise
https://theboostcpplibraries.com/boost.asio-network-programming
Develop a client and a server which can transfer a file from one computer to another. 
When the server is started, it should display a list of IP addresses of all local interfaces and wait for
the client to connect.

When the client is started, an IP address from the server and the name of a local 
file should be passed as command line options. 

The client should transfer the file to the server which saves 
it to the current working directory. During transmission the client should display some sort of progress indicator 
so that the user knows that the transmission is ongoing. 
Implement the client and server with callbacks.
***************************************************************************************************************/

#include <cstdlib>
#include <iostream>
#include <list>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include <fstream>
#include <ctime>

using boost::asio::ip::tcp;
using namespace std;


// 获取当前系统时间字符串
static std::string get_nowtime_str()
{
	time_t tm = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y_%m_%d_%H_%M_%S", localtime(&tm));
	puts(tmp);

	std::string strResult(tmp);
	
	return strResult;
}

// 接收客户端传送过来的文件
class FileRecv_Session :
	public boost::enable_shared_from_this<FileRecv_Session>
{
public:
	FileRecv_Session(boost::asio::io_context& io_context)
		: socket_(io_context)
	{
		fileName_ = get_nowtime_str() + ".txt";
	}

	tcp::socket& socket()
	{
		return socket_;
	}

	void start()
	{
		do_read_fileLen();
	}

	// 从客户端获取文件大小
	void do_read_fileLen()
	{
		auto self(shared_from_this());
		auto read_cb = [this, self](boost::system::error_code e, std::size_t bytes_read) {
			if (!e)
			{
				char szBuffer[1024] = { 0 };
				// 一次最多接收1024字节的数据

				
				fileLen_ = bytes_read;	// 获取文件大小
				do_get_fileContent();
			}
			else {
				do_read_fileLen();
			}
		};

		socket_.async_read_some(boost::asio::buffer(buffer_, max_length), read_cb);
	}

	// 从客户端获取文件内容
	void do_get_fileContent()
	{
		auto self(shared_from_this());
		auto read_cb = [this, self](boost::system::error_code e, std::size_t bytes_read) {
			if (!e)
			{
				// 一次最多接收1024字节的数据
				while (fileLen_ > 0)
				{
					handle_SaveFile(buffer_, bytes_read);

					fileLen_ -= max_length;

					do_get_fileContent();
				}
			}
			else {
				do_get_fileContent();
			}
		};

		socket_.async_read_some(boost::asio::buffer(buffer_, max_length), read_cb);
	}

	// 保存文件，目前只支持txt格式
	void handle_SaveFile(char *buffer, std::size_t len)
	{
		// 将接收到的数据保存到文件中
		/*std::string fileOutPath;
		fileOutPath = get_nowtime_str() + ".txt";

		if (fileOutPath == "")
		{
			fileOutPath = "tmp.txt";
		}*/

		ofstream fout(fileName_, ios::app | ios::binary);
		if (fout)	// 如果文件创建成功
		{
			fout.write(buffer, len);
			//fout << buffer << endl;
			//cout << buffer << endl;
		}
		
		fout.close();
	}



private:
	tcp::socket socket_;
	static int fileLen_;			// 接收的文件大小
	enum { max_length = 1024 };
	char buffer_[max_length];
	int data_length_ = 0;
	std::string fileName_;
};

int FileRecv_Session::fileLen_ = 0;

typedef boost::shared_ptr<FileRecv_Session> FileRecv_Session_Ptr;

class File_server
{
public:
	File_server(boost::asio::io_context& io_context,
		const tcp::endpoint& endpoint)
		: io_context_(io_context),
		acceptor_(io_context, endpoint)
	{
		start_accept();
	}

	void start_accept()
	{
		FileRecv_Session_Ptr new_session(new FileRecv_Session(io_context_));
		acceptor_.async_accept(new_session->socket(),
			boost::bind(&File_server::handle_accept, this, new_session,
				boost::asio::placeholders::error));
	}

	void handle_accept(FileRecv_Session_Ptr session,
		const boost::system::error_code& error)
	{
		if (!error)
		{
			session->start();
		}

		start_accept();
	}

private:
	boost::asio::io_context& io_context_;
	tcp::acceptor acceptor_;
};

//----------------------------------------------------------------------
typedef boost::shared_ptr<File_server> File_server_ptr;
typedef std::list<File_server_ptr> File_server_list;


int main(int argc, char* argv[])
{
	
	try
	{
		if (argc < 2)
		{
			std::cerr << "Usage: File_server <port> [<port> ...]\n";
			return 1;
		}

		boost::asio::io_context io_context;

		File_server_list servers;
		for (int i = 1; i < argc; ++i)
		{
			using namespace std; // For atoi.
			tcp::endpoint endpoint(tcp::v4(), atoi(argv[i]));
			File_server_ptr server(new File_server(io_context, endpoint));
			servers.push_back(server);
		}

		io_context.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	std::system("pause");

	return 0;
}
