   IProgramService是服务程序的接口类，为派生类提供了run()和stop()接口，可以根据自己的项目需要从IProgramService派生一个类，比如MainService，MainService需要实现
run()和stop()这两个函数。
 ```cpp
class MainService : public IProgramService
{
public:
	MainService();
	~MainService();

	void run() override;

	void stop() override;
};
```
可以在主函数中调用：
```cpp
#include <iostream>
#include "MainService.h"
#include "WinSystemService.h"

int main(int argc, char* argv[])
{
	MainService ms;
	
	if (argc > 1)
	{
		WinSystemService svc((IProgramService*)&ms, "my_service", "my_service", "Windows服务程序示例");
		svc.Control(argc, argv);
		return 0;
	}

 return 0;
 
}
```
