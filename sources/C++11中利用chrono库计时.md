 # [C++11中利用chrono库计时](https://ccf19881030.blog.csdn.net/article/details/132484165)
   众所周知，游戏、3D图形学、音视频等领域对性能是比较讲究的。很多时候需要对某个功能代码进行耗时性能分析，此时需要计时处理。在油管上看到Up主The Cherno的一集视频[Timing in C++]( https://www.youtube.com/watch?v=oEx5vGNFrLk)，巧妙利用std::chrono 库的一些方法，将其包装在对象中，并充分地利用作用域生命周期来发挥其优势，达到对某个函数进行计时的目的。由于chrono库是跨平台的，所以很方便地移植到Widows、Mac、Linux等多种平台。
 
## 1、C++11中对某个代码段进行计时
首先，让我们看一下chrono库对某段代码的执行时间，使用到了高精度定时器`high_resolution_clock`，进行简单计时，代码如下：
```cpp
#include <iostream>
#include <chrono>
#include <thread>

int main()
{
	using namespace std::literals::chrono_literals;

	auto start = std::chrono::high_resolution_clock::now();
	std::this_thread::sleep_for(1s);
	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<float> duration = end - start;
	std::cout << duration.count() << "s " << std::endl;

	std::cin.get();

	return 0;
}
```
在VS2017中运行结果如下：
![对某个代码段记性计时](https://img-blog.csdnimg.cn/c98909a81e214529a80873fa4b399d6d.png)
当然，上面的代码段用start记录了开始时间点，然后延时1秒，最后用end记录了结束时间点，用end -start得到的duration就是时间间隔，即std::this_thread::sleep_for(1s);执行的时间。不过系统最终的执行结果`1.00442s`有一点误差，这是正常的。

## C++11中对某个函数进行计时
利用std::chrono 库的一些方法，将其包装在对象中，并充分地利用作用域生命周期来发挥其优势，达到对某个函数进行计时的目的。首先我们可以定义一个定时器`Timer`结构体，定义`start`，`end`两个成员变量分别表示开始时间和结束时间，duration表示Timer类对象的生命周期。在构造函数中给`start`变量赋值为当前时间；在析构函数中给`end`变量赋值为当前时间，然后计算出duration。代码段如下：
 ```cpp
 #include <iostream>
#include <chrono>
#include <thread>

struct Timer
{
	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::time_point<std::chrono::steady_clock> end;
	std::chrono::duration<float> duration;

	Timer()
	{
			start = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
			end = std::chrono::high_resolution_clock::now();
			duration = end - start;
			auto ms = duration.count() * 1000;
			std::cout << "Timer costs " << ms << " ms " << std::endl;
	}
};

void Function()
{
	Timer timer;

	for (int i = 0; i < 1000; i++) {
		std::cout << "hello " << i << std::endl;
	}
}

int main()
{
	Function();

	std::cin.get();

	return 0;
}
```

运行结果如下图所示：
![对某个函数计时](https://img-blog.csdnimg.cn/5a12e7be495d45f0bb551f3d2fe29aaf.png)
