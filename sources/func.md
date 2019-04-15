# Cplus_libs_wrapper
fun.h中相关函数的使用

## 1、C++11 std::chrono库实现获取当前系统时间，自定义一个定时器类
   位于sources/fun.h
### 1、定时器类Timer
```
#include "fun.h"  // 包含头文件

Math::Timer timer;

// 自定义一个定时器入口函数，函数声明如：typedef std::function<void()> timer_handler;
void timer_func()
{

    std::cout << "timer begin:" << std::endl;
    for (int i = 0; i < 1000; ++i)
    {
         std::cout << "count = " << i <<" in timer func\n";
    }
    std::cout << "timer end:" << std::endl;
}
// 启动定时器，间隔定时5秒
timer.start(std::bind(timer_func), 10);
```
### 2、获取当前时间字符串，默认格式 2018-10-01 01:10:20
   ```
   #include "fun.h"  // 包含头文件
    // 获取当前时间字符串，默认格式 2018-10-01 01:10:20
   Math::Date::getnow("%04d.%02d.%02d %02d:%02d:%02d");
   ```
