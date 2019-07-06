## [VC++DLL动态链接库编程](https://study.163.com/course/courseMain.htm?courseId=1003586013)
### [课时1-课程介绍](https://study.163.com/course/courseMain.htm?courseId=1003586013)
### [课时2-DLL动态库的原理讲解](https://study.163.com/course/courseLearn.htm?courseId=1003586013#/learn/video?lessonId=1004088362&courseId=1003586013)
### [课时3-VC++静态库编译](https://study.163.com/course/courseLearn.htm?courseId=1003586013#/learn/video?lessonId=1004086375&courseId=1003586013)
### [课时4-DLL动态链接库创建](https://study.163.com/course/courseLearn.htm?courseId=1003586013#/learn/video?lessonId=1004087388&courseId=1003586013)
### [课时5-直接通过代码访问DLL中的函数](https://study.163.com/course/courseLearn.htm?courseId=1003586013#/learn/video?lessonId=1004090372&courseId=1003586013)

### 静态库示例程序



### 动态库示例程序
#### 1.DllDemo dll程序
DllDemo.h
```cpp
// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 DLLDEMO_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// DLLDEMO_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef DLLDEMO_EXPORTS
#define DLLDEMO_API __declspec(dllexport)
#else
#define DLLDEMO_API __declspec(dllimport)
#endif

// 此类是从 DllDemo.dll 导出的
class DLLDEMO_API CDllDemo {
public:
	CDllDemo(void);
	// TODO:  在此添加您的方法。
};

extern DLLDEMO_API int nDllDemo;

// 加上extern "C"表示是一个C函数，不重载
extern "C" DLLDEMO_API int fnDllDemo(void);
```
DllDemo.cpp
```cpp
// DllDemo.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "DllDemo.h"

#include <iostream>
using namespace std;

// 这是导出变量的一个示例
DLLDEMO_API int nDllDemo=0;

// 这是导出函数的一个示例。
DLLDEMO_API int fnDllDemo(void)
{
	cout << "fnDllDemo(void) called" << endl;

	return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 DllDemo.h
CDllDemo::CDllDemo()
{
	cout << "CDllDemo::CDllDemo() called" << endl;

	return;
}
```
dllmain.cpp
```cpp
// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
```

#### 使用DLL的测试程序 testDllDemo
testDllDemo.cpp
```cpp
// testDllDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <stdio.h>
#include <Windows.h>

typedef int(*dllpFun)(void);

int _tmain(int argc, _TCHAR* argv[])
{
	HMODULE hDll = LoadLibrary(L"DllDemo.dll");

	if (!hDll)
	{
		return -1;
	}

	dllpFun pFunc = (dllpFun)GetProcAddress(hDll, "fnDllDemo");

	pFunc();

	FreeLibrary(hDll);

	getchar();

	return 0;
}
```
