// dllUser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "wuygDll.h"
#include <iostream>
#include <windows.h>

#pragma comment(lib, "wuygDll")

int _tmain(int argc, _TCHAR* argv[])
{
	//��ʽ����
	wuygFun();
	
	//��ʽ����
	typedef int (__stdcall *Fun)(void); 
	HMODULE hDll = ::LoadLibrary(L"wuygDll");
	Fun TestFun = ::GetProcAddress(hDll, "wuygFun");
	TestFun();
	system("pause");
	return 0;
}

