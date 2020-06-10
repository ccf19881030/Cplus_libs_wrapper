// MatureApproach.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "MatureApproach.h"
#include "ExportClassImpl.h"


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

MATUREAPPROACH_API IExport* APIENTRY CreateExportObj()
{
	return new ExportImpl;
}


//这里不能直接delete pExport，因为没有把IExport的析构函数定义为虚函数
MATUREAPPROACH_API void  APIENTRY DestroyExportObj(IExport* pExport)
{
	pExport->Release();
}