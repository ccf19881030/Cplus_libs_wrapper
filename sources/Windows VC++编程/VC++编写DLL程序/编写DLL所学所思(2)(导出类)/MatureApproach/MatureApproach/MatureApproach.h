//2011.10.6
//cswuyg
//dll导出类
//dll跟其使用者共用的头文件
#pragma  once
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MATUREAPPROACH_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MATUREAPPROACH_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MATUREAPPROACH_EXPORTS
#define MATUREAPPROACH_API __declspec(dllexport)
#else
#define MATUREAPPROACH_API __declspec(dllimport)
#endif

class IExport 
{
public:
	virtual void Hi() = 0;
	virtual void Test() = 0; 
	virtual void Release() = 0;
};


extern "C" MATUREAPPROACH_API IExport* _stdcall CreateExportObj();
extern "C" MATUREAPPROACH_API void _stdcall DestroyExportObj(IExport* pExport);

