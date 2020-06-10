// debug_xunlei.cpp : �������̨Ӧ�ó������ڵ㡣
//vs2008 unicode
// Ѹ��API��ʵ���ļ����أ�https://www.cnblogs.com/corerman/p/4726933.html
//  Ѹ�����ؿ������� ����ʵ����http://xldoc.xl7.xunlei.com/0000000026/index.html
#include "stdafx.h"
#include "debug_xunlei.h"

#include <stdio.h>
#include "stdlib.h"
#include <windows.h>
#include <tchar.h>
#include "XLDownload.h"
#include "XLError.h"
#pragma comment(lib, "XLDownload.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	// 1����ʼ����������
	if ( FALSE == XLInitDownloadEngine() )
	{
		printf("Initialize download engine failed.\n");
		return 1;
	}
/*	 system("pause");
	LONG id=0;
	int test1=XLContinueTaskFromTdFile(TEXT("d:\\xmp.exe.td"),id);
	if(test1==XL_SUCCESS)
		printf("OK");
	while(1)
	{
		Sleep(1000);
		LONG a;
		ULONGLONG b,c;
		test1=XLQueryTaskInfo(id,&a,&b,&c);
		while(test1==XL_SUCCESS)
		{
			Sleep(1000);
			test1=XLQueryTaskInfo(id,&a,&b,&c);
			if(a==enumTaskStatus_Success)
				return 1;
		}
	}

	
	system("pause");
*/

	// 2������������
	// PS����������޷�ͨ������ѹ������ԡ���wchar_t����Ϊ��������        (/Zc:wchar_t)������ΪYes
	LONG   lTaskId = 0;
	DWORD  dwRet   = XLURLDownloadToFile(
		_T("d:\\xshell.exe"), 
		_T("http://xmp.down.sandai.net/kankan/XMPSetup_3.8.1.485-www.exe"), 
		_T(""), lTaskId);

	printf("Begin download file.\n");

	// 3����ѯ����״̬
	do 
	{
		::Sleep(1000);
		system("cls"); //windows ����
		ULONGLONG   ullFileSize = 0;
		ULONGLONG   ullRecvSize = 0;
		LONG        lStatus     = -1;

		dwRet = XLQueryTaskInfo(lTaskId, &lStatus, &ullFileSize, &ullRecvSize);
		if ( XL_SUCCESS==dwRet )
		{
			// ���������Ϣ
			if ( 0 != ullFileSize )
			{
				double  douProgress = (double)ullRecvSize/(double)ullFileSize;
			//	printf("%.2lfMB",(double)ullRecvSize/8/1024/1024);
				douProgress *= 100.0;
				printf("Download progress:%.2f%%   %.2lfMB\n", douProgress,(double)ullRecvSize/1024/1024);
			}
			else
			{
				printf("File size is zero.\n");
			}

			if ( enumTaskStatus_Success==lStatus )
			{
				printf("Download successfully.\n");
				break;
			}

			if ( enumTaskStatus_Fail==lStatus )
			{
				printf("Download failed.\n");
				break;
			}
		}
	} while( XL_SUCCESS==dwRet );

	// 4�������Ƿ����سɹ������������XLStopTask
	XLStopTask(lTaskId);

	// 5���ͷ���Դ
	XLUninitDownloadEngine();
	return 0;
}
