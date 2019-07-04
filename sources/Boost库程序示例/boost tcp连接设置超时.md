## [boost tcp连接设置超时](https://www.jianshu.com/p/5646af2e8c1a)
本文参考了用[asio的定时器实现带超时的connect](https://link.jianshu.com/?t=http%3A%2F%2Fwww.cnblogs.com%2Flidabo%2Fp%2F3785800.html)，备忘，做了一些改进。
把代码贴在这里吧：
```cpp
//
//=====================================================================================================
//
#pragma once
//
//=====================================================================================================
//
#include <boost/asio.hpp>  
#include <boost/bind.hpp>  
#include <boost/date_time/posix_time/posix_time_types.hpp>  

using boost::asio::ip::tcp;
using boost::asio::io_service;
using boost::asio::deadline_timer;

class AsyncConnectHandler
{
public:
    AsyncConnectHandler(io_service& ios, tcp::socket &s)
        :io_service_(ios)
        , timer_(ios)
        , socket_(s) {}
    bool aysnc_connect(const tcp::endpoint &ep, int million_seconds) {
        bool connect_success = false;
        socket_.async_connect(ep, boost::bind(&AsyncConnectHandler::connect_handle, this, _1, boost::ref(connect_success)));
        timer_.expires_from_now(boost::posix_time::milliseconds(million_seconds));
        bool timeout = false;
        timer_.async_wait(boost::bind(&AsyncConnectHandler::timer_handle, this, _1, boost::ref(timeout)));
        do
        {
            io_service_.run_one(); // 等待异步操作完成
        } while (!timeout && !connect_success); // 这两个异步操作有一个完成则不再等待
        timer_.cancel();
        return connect_success;
    }
protected:
private:
    void connect_handle(boost::system::error_code ec, bool &connect_success) {
        if (!ec)
        {
            // 连接成功
            connect_success = true;
        }
    }

    void timer_handle(boost::system::error_code ec, bool &timeout) {
        if (!ec)
        {
            // 定时器超时
            socket_.close();
            timeout = true;
        }
    }

    io_service &io_service_;
    deadline_timer timer_;
    tcp::socket &socket_;
};
```
使用代码为：
```cpp
try
    {
        io_service ios;
        tcp::socket s(ios);
        AsyncConnectHandler acHandler(ios, s);

        tcp::endpoint ep(ip::address_v4::from_string("192.168.88.8"), 7199);
//      tcp::endpoint ep(ip::address_v4::loopback(), 7199);
        if (acHandler.aysnc_connect(ep, 3000))
        {
            std::cout << "connect success\n";
            // send/recv
            ios.run();
        }
        else {
            std::cout << "connect failed\n";
        }

    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    ```
