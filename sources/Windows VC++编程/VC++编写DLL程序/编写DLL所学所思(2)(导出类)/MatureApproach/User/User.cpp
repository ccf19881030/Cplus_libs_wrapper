// User.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MatureApproach.h"

#pragma comment(lib, "../debug/MatureApproach.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	IExport* pExport = CreateExportObj();
	pExport->Hi();
	pExport->Test();
	DestroyExportObj(pExport);
	system("pause");
	return 0;
}

