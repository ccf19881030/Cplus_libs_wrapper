// User.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "NaiveApproach.h"
#pragma comment(lib, "../debug/NaiveApproach.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	//////////////////////////////////////////////////////////////////////////
	CNaiveApproach obj(10);
	obj.Test();
	obj.Func();
	//////////////////////////////////////////////////////////////////////////
	return 0;
}

