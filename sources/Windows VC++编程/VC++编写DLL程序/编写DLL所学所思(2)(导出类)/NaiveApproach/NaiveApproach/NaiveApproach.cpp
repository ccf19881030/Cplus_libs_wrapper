// NaiveApproach.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "NaiveApproach.h"


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


// This is the constructor of a class that has been exported.
// see NaiveApproach.h for the class definition
CNaiveApproach::CNaiveApproach(int i) : m_iwuyg(i)
{
}

void CNaiveApproach::Func()
{
	wcout << L" cswuyg test dll , i = " << m_iwuyg << endl;
	system("pause");
}

void CBase::Test()
{
	wcout << L"Just a  Test" << endl;
	system("pause");
}

void CDate::Test2()
{
	wcout << L"Test 2" << endl;
	system("pause");
}