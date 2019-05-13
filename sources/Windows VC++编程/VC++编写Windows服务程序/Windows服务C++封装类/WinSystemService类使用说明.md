   IProgramService是服务程序的接口类，为派生类提供了run()和stop()接口，可以根据自己的项目需要从IProgramService派生一个类，比如MyService，MyService必须要重写run()和stop()这两个函数。
 ```cpp
class MyService : public IProgramService
{
public:
	MyService();
	~MyService();

	void run() override;

	void stop() override;
};
```
可以在主函数中调用：
```cpp
#include <iostream>
#include "MyService.h"
#include "WinSystemService.h"

int main(int argc, char* argv[])
{
	MyService ms;
	
	if (argc > 1)
	{
		WinSystemService svc((IProgramService*)&ms, "my_service", "my_service", "Windows服务程序示例");
		svc.Control(argc, argv);
		return 0;
	}

 return 0;
 
}
```
