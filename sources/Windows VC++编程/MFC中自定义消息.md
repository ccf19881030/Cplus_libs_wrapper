## [MFC中自定义消息](https://www.cnblogs.com/sgdd123/p/7868297.html)
### 1、在头文件stdafx.h中增加一个自定义消息宏
#define WM_USER_THREADEND WM_USER + 1

### 2、在于增加新消息的窗口或对话框类的头文件中增加一个回调函数声明
afx_msg LRESULT OnUserThreadend(WPARAM wParam, LPARAM lParam);
注意：这个函数对返回值和传递参数是有要求的，不可以随意修改。

### 3、在窗口或对话框的cpp文件的BEGIN_MESSAGE_MAP，END_MESSAGE_MAP 中增加一行
ON_MESSAGE(WM_USER_THREADEND, OnUserThreadend)

### 4、在窗口或对话框的cpp文件中增加回调函数的实现，如：
LRESULT ThreadDialog::OnUserThreadend(WPARAM wParam, LPARAM lParam)
{
TRACE("WM_USER_THREADEND message /n");
return 0;
}

### 5、自定义消息的触发
::PostMessage(GetSafeHwnd(), WM_USER_THREADEND, 0, 0);
其中GetSafeHwnd()得到了一个当前窗口的句柄，此消息将发给当前窗口，如果想发送消息给其它窗口只需改变这个句柄，前提是目的窗口也实现了此消息的处理函数。
说明：使用全局函数::PostMessage()不一定会调用指定的类函数。建议先获取该类的指针，然后调用该类的PostMessage()函数，确保消息是向这个类发送的
