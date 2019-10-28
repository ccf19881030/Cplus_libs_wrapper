信号槽是Qt框架中一个重要的部分，主要用来解耦一组互相协作的类，使用起来非常方便。项目中有同事引入了第三方的信号槽机制，其实Boost本身就有信号/槽，而且Boost的模块相对来说更稳定。

signals2基于Boost里另一个库signals实现了线程安全的观察者模式。signal中一个比较重要的操作函数是connect，它把插槽连接到信号上；插槽可以是任意可调用对象，包括函数指针、函数对象，以及他们的bind/lambda表达式和function对象。connect函数将返回一个connection对象，表示了信号和插槽之间的连接关系，connection对象可以更灵活的处理信号与槽函数的连接、断开等关系。
以下是一个使用Boost信号/槽的小例子：　
```cpp
#include <iostream>

#include <boost/signals2.hpp>
#include <boost/bind.hpp>
#include <boost/optional/optional_io.hpp> 

using namespace boost::signals2;

using namespace std;

void slots1()
{
	cout << "slot1 called" << endl;
}

void slots2()
{
	cout << "slot2 called" << endl;
}

void slot_test()
{
	boost::signals2::signal<void()> sig_void;	// 一个信号对象

	sig_void.connect(&slots1);	// 连接插槽1
	sig_void.connect(&slots2, boost::signals2::at_front);  // 连接插槽2
	sig_void();				    // 调用operator(),产生信号(事件)，触发插槽调用
}

template<int N>
struct  slots
{
	int operator() (int x)
	{
		cout << "slot " << N << " called" << endl;
		return x * N;
	}
};

void sigl2_test2()
{
	boost::signals2::signal<int(int)> sig;
	sig.connect(slots<10>());
	sig.connect(slots<20>());
	sig.connect(slots<50>());

	cout << *sig(2) << endl;
}

class A
{
public:
	static int staticMemberFunc(int param)
	{
		cout << "A::staticMemberFunc(int param) called, param: " << param << endl;

		return 0;
	}

	int memberFunc(int param)
	{
		cout << "A::memberFunc(int param) called, param: " << param << endl;

		return 0;
	}
};

void sig3_test()
{
	boost::signals2::signal<void()> sig_void;
	boost::signals2::signal<int(int)> sig2_int;

	A a;
	boost::signals2::connection conn1 = sig_void.connect(&slots1);
	boost::signals2::connection conn2 = sig_void.connect(&slots2);
	cout << "First call-------------------" << endl;
	sig_void();

	if (conn2.connected())
	{
		conn2.disconnect();
	}
	cout << "Second call-------------------" << endl;
	sig_void();

	boost::signals2::connection conn3 = sig2_int.connect(&A::staticMemberFunc);	// 绑定静态成员函数
	boost::signals2::connection conn4 = sig2_int.connect(boost::bind(&A::memberFunc, &a, _1)); // 绑定成员函数

	cout << "Return code is: " << sig2_int(44) << endl;	// 只能返回最后被调用的插槽的返回值
}

int main()
{
	slot_test();
	sigl2_test2();
	sig3_test();

	return 0;
}
```
注意使用解引用操作符*获取的只是最后被调用的插槽的返回值，如果需要知道每个插槽函数的返回值需要使用合并器（combiner）。

Boost的信号/槽在信号被触发时，槽函数只能是同步执行，没有像Qt那样的异步接口。Qt异步的实现实际上是将信号push到一个队列中，然后由统一的线程来处理信号对应的槽函数而已。当然也可以根据这个原理自己封装带异步的信号/槽机制，不过那样的话应该需要另外开启线程了。

