//2011.10.6
//cswuyg
//dll导出类，比较差劲的方法
#pragma once
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the NAIVEAPPROACH_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// NAIVEAPPROACH_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef NAIVEAPPROACH_EXPORTS
#define NAIVEAPPROACH_API __declspec(dllexport)
#else
#define NAIVEAPPROACH_API __declspec(dllimport)
#endif



//基类也必须导出，否则警告：
class  NAIVEAPPROACH_API CBase
{
public:
	void Test();
private:
	int m_j;
};

//也必须导出
class NAIVEAPPROACH_API CDate
{
public:
	void Test2();
private:
	int m_k;
};

class NAIVEAPPROACH_API CNaiveApproach : public CBase
{
public:
	CNaiveApproach(int i = 0);
	// TODO: add your methods here.
	void Func();
private:
	int m_iwuyg;
	CDate m_dobj;
};

