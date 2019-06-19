## [Windows操作系统下 使用c++ WIN32API禁用控制台最小化和关闭按钮](https://www.cnblogs.com/zhangdewang/p/10032285.html)
```cpp
#include<Windows.h>    

 //屏蔽控制台最小按钮和关闭按钮
 HWND hwnd = GetConsoleWindow();
 HMENU hmenu = GetSystemMenu(hwnd, false);
 RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
 LONG style = GetWindowLong(hwnd, GWL_STYLE);
 style &= ~(WS_MINIMIZEBOX);
 SetWindowLong(hwnd, GWL_STYLE, style);
 SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
 ShowWindow(hwnd, SW_SHOWNORMAL);
 DestroyMenu(hmenu);
 ReleaseDC(hwnd, NULL);
 ```
 
 ## [VC++无效化控制台窗口的关闭按钮(菜单)](https://blog.csdn.net/sunnysab/article/details/9367037)
 控制台窗口的句柄为hwnd的话，将关闭按钮无效化的完整代码为：
 ```cpp
HWND hwnd = GetConsoleWindow();
HMENU hMenu = GetSystemMenu(hwnd,false);
EnableMenuItem(hMenu,SC_CLOSE,MF_GRAYED|MF_BYCOMMAND);
```
同理，还可以将窗口的其他属性灰化和无效化。
