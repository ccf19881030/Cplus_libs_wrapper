## [VC++ 删除一个文件目录下的所有文件以及目录](https://www.cnblogs.com/chechen/p/4159027.html)
```cpp
BOOL DoRemoveDirectory(CString chrDirName);

BOOL ReleaseDirectory(CString chrDirName)
{
    BOOL bRemove = DoRemoveDirectory(chrDirName);
    if (bRemove)
        ::RemoveDirectory(chrDirName);
    else
        return FALSE;

    return TRUE;

} //end of CIVMSvrInfo::ReleaseDirectory()

BOOL DoRemoveDirectory(CString chrDirName)
{
    CString strDirName = chrDirName;

    BOOL result;
    HANDLE Handle;
    WIN32_FIND_DATA fData;
    CString strTemp;
    DWORD errorcode;

    Handle = FindFirstFile(strDirName + "\\*.*", &fData);

    if (Handle == INVALID_HANDLE_VALUE)
        return FALSE;

    do {
        errorcode = GetLastError();

        if ( fData.cFileName[0] == '.' )
            continue;

        if (fData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
            if (strDirName[strDirName.GetLength()-1] != '\\'){
                TCHAR chA[MAX_PATH];
                CString strA = strDirName+'\\'+ fData.cFileName;
                lstrcpy(chA, strA);
                DoRemoveDirectory(chA);
            }
            else{
                TCHAR chB[MAX_PATH];
                CString strB = strDirName + fData.cFileName;
                lstrcpy(chB, strB);
                DoRemoveDirectory(chB);
            }

            strTemp = strDirName + "\\" + fData.cFileName;

            SetFileAttributes(strTemp, ~FILE_ATTRIBUTE_READONLY);
            if (!RemoveDirectory(strTemp))
                result = FALSE;
            else
                result = TRUE;
        }
        else
        {
            strTemp = strDirName + "\\" + fData.cFileName;
            BOOL bl = SetFileAttributes(strTemp, ~FILE_ATTRIBUTE_READONLY);

            if (!DeleteFile(strTemp))
                result = FALSE;
            else
                result = TRUE;
        }

    }while(FindNextFile(Handle,&fData));

    errorcode = GetLastError();

    if (errorcode == ERROR_NO_MORE_FILES)//空目录
    {
        ::RemoveDirectory(strDirName);
        result = TRUE;
    }

    if (Handle)
        FindClose(Handle);

    return result;

}    // end of BOOL PreRemoveDirectory(CString DirName)

```
