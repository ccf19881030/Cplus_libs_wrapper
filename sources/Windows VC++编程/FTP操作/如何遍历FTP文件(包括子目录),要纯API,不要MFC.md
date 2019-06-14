## [如何遍历FTP文件(包括子目录),要纯API,不要MFC](https://bbs.csdn.net/topics/380178547)
```cpp
//vc遍历文件夹
#include "stdafx.h"
#include <string>
#include <Windows.h>
#include <list>
#include <io.h>
 
using namespace std;
 
BOOL FindAllFiles(string strDirectory, list<string> &ml, string &strALL);
int _tmain(int argc, _TCHAR* argv[])
{
    string strDirectory = "F:\\qudong";
    list<string> ml;
    string strALL = "";
    FindAllFiles(strDirectory, ml, strALL);
    return 1;
}
//参数1：文件夹路径 参数2：获取的所有文件名 参数3：获取的所有文件名字符串（用\n分开）
BOOL FindAllFiles(string strDirectory, list<string> &ml, string &strALL)
{
    _finddata_t c_file;
    c_file.attrib =_MAX_PATH;
    string strTmpPath =  strDirectory + "\\*.*";
    intptr_t hFile = _findfirst(strTmpPath.c_str(), &c_file);
    if((HANDLE)hFile == INVALID_HANDLE_VALUE)
    {
        return TRUE;
    }
    do
    {
        if( c_file.attrib & _A_SUBDIR && string(c_file.name) != "." && string(c_file.name) != ".." && !(c_file.attrib & _A_SYSTEM) )
        {
 
            FindAllFiles(strDirectory + "\\" + c_file.name, ml, strALL);
        }
        else
        {
            if(string(c_file.name) != "." && string(c_file.name) != "..")
            {
                if(c_file.size > 0)
                {
                    ml.push_back(strDirectory + "\\" + c_file.name);
                    strALL += strDirectory + "\\" + c_file.name + "\n";
                }
            }
        }
    }while(_findnext(hFile, &c_file) == 0);
    _findclose(hFile);
    return TRUE;
}
```
//枚举FTP
```cpp
//枚举FTP
void EnumFiles(HINTERNET hFtp, vector<CString>& strPaths, int iPath, vector<CString>& strFiles)
{
    CString strPath = "";
    if (!strPaths.empty())
        strPath = "/" + strPaths[iPath];
    FtpSetCurrentDirectory(hFtp, strPath);
    WIN32_FIND_DATA fd;
    HINTERNET hFind = FtpFindFirstFile(hFtp, "*.*", &fd, 0, 0);
    while (hFind)
    {
        if (strcmp(fd.cFileName, ".") == 0 || strcmp(fd.cFileName, "..") == 0)
        {
            if (!InternetFindNextFile(hFind, &fd))
                break;
            continue;
        }
        CString strRemoteFile;
        if (!strPath.IsEmpty())
            strRemoteFile = strPaths[iPath] + "/" + fd.cFileName;
        else
            strRemoteFile = fd.cFileName;
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            strPaths.push_back(strRemoteFile);
            if (!InternetFindNextFile(hFind, &fd))
                break;
            continue;
        }
        CString strLocalFile = strRemoteFile;
        strLocalFile.Replace("/", "\\");
        if (::PathFileExists(strLocalFile))
        {//比较文件大小,可根据实际需要比较日期,版本等.
            DWORD nFileSize = 0;
            WIN32_FIND_DATA wfd;
            HANDLE hFile = FindFirstFile(fd.cFileName, &wfd);
            if (hFile != INVALID_HANDLE_VALUE)
            {
                nFileSize = wfd.nFileSizeLow;
                FindClose(hFile);
            }
            if (nFileSize == fd.nFileSizeLow)
            {
                if (!InternetFindNextFile(hFind, &fd))
                    break;
                continue;
            }
        }
        strFiles.push_back(strRemoteFile);
        if (!InternetFindNextFile(hFind, &fd))
            break;
    }
    if (hFind)
        InternetCloseHandle(hFind);
}
 
BOOL FTPDownloadFiles(LPCTSTR lpszIP, DWORD dwPort, LPCTSTR lpszUID, LPCTSTR lpszPWD)
{
    ///////////////////////
    //open ftp directoy and get needs update files
    HINTERNET hInet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, 0,
        INTERNET_FLAG_DONT_CACHE);//打开目录,不缓存
    HINTERNET hFtp = InternetConnect(hInet, lpszIP, dwPort, lpszUID, lpszPWD, INTERNET_SERVICE_FTP,
        INTERNET_FLAG_PASSIVE, 0);//FTP,被动模式
    vector<CString> strPaths;//路径暂存
    vector<CString> strFiles;//需要下载的文件列表
    EnumFiles(hFtp, strPaths, 0, strFiles);
    for (int i = 0; i < strPaths.size(); i ++)
    {
        EnumFiles(hFtp, strPaths, i, strFiles);//枚举FTP文件
    }
    //根据strFiles的内容下载文件...
   ```
