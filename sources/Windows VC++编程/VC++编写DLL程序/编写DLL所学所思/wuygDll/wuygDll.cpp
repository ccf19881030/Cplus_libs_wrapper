// wuygDll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "wuygDll.h"
#include <iostream>
using namespace std;



#ifdef _MANAGED
#pragma managed(push, off)
#endif


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

#ifdef _MANAGED
#pragma managed(pop)
#endif

extern "C" int _stdcall wuygFun( void )
{
	wcout << L"Hello This a DLL Test by cswuyg" << endl;
	return 0;
}