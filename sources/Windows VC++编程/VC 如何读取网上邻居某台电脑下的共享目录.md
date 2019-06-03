## [VC 如何读取网上邻居某台电脑下的共享目录](https://bbs.csdn.net/topics/392048596?list=lz)
现在已经获取电脑的网络名,如\\n0,\\n8  等 .请问如何获取 \\n8 下的共享文件夹以及文件呢?

经测试 ,用这个方法 	hFind = FindFirstFile(strPath, &FindFileData);  读取不了任何信息.

非常感谢!!!
暴露了你不会在VC里面调用命令行工具。

参考下面：
```cpp
system("dir /b /a-d c:\\*.* >d:\\allfiles.txt");
//读文件d:\\allfiles.txt的内容即C:\\下所有文件的名字
system("dir /b /a-d /s c:\\*.* >d:\\allfilesinsub.txt");
//读文件d:\\allfilesinsub.txt的内容即C:\\下所有文件的名字包含子目录
system("dir /b /ad  c:\\*.* >d:\\alldirs.txt");
//读文件d:\\alldirs.txt的内容即C:\\下所有子目录的名字
请记住，能用shell命令获取文件、文件夹信息或者操作文件、文件夹最好用shell命令获取或者操作，而不要用各种API获取或者操作，因为当遇到非法文件夹名或非法文件名或非法文件长度、非法文件日期、压缩文件、链接文件、稀疏文件……等各种意料之外的情况时，API会处理的不全面或陷入死循环，而shell命令不会。
如果嫌system黑窗口一闪，将system("...")替换为WinExec("cmd /c ...",SW_HIDE);
```
