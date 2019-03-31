#pragma  once

/*
------------------��������--------------
*/

BOOL  __stdcall XLInitDownloadEngine();

DWORD __stdcall XLURLDownloadToFile(LPCTSTR pszFileName, LPCTSTR pszUrl, LPCTSTR pszRefUrl, LONG & lTaskId);

DWORD __stdcall XLQueryTaskInfo(LONG lTaskId, LONG *plStatus, ULONGLONG *pullFileSize, ULONGLONG *pullRecvSize);

DWORD __stdcall XLPauseTask(LONG lTaskId, LONG & lNewTaskId);

DWORD __stdcall XLContinueTask(LONG lTaskId);

DWORD __stdcall XLContinueTaskFromTdFile(LPCTSTR pszTdFileFullPath, LONG & lTaskId);

VOID  __stdcall XLStopTask(LONG lTaskId);

BOOL  __stdcall XLUninitDownloadEngine();

DWORD __stdcall XLGetErrorMsg(DWORD dwErrorId, LPTSTR pszBuffer, DWORD & dwSize);


/*
----------------------���Ͷ���-----------------
*/
enum enumTaskStatus{
	enumTaskStatus_Connect = 0,                 // �Ѿ���������
	enumTaskStatus_Download = 2,                // ��ʼ���� 
	enumTaskStatus_Pause = 10,                  // ��ͣ
	enumTaskStatus_Success = 11,                // �ɹ�����
	enumTaskStatus_Fail = 12,                   // ����ʧ��
};