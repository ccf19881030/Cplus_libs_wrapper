## 一、[MFC中自定义消息](https://www.cnblogs.com/sgdd123/p/7868297.html)
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

## 二、[mfc自定义消息 wparam lparam 参数传递指针](https://blog.csdn.net/s200820212/article/details/31780805)
1.在.cpp文件中添加
#define  WM_MY_DELETEITEM  WM_USER+100

2.在.h文件

afx_msg LRESULT OnDeleteItem(WPARAM,LPARAM);

3、在.cpp文件中添加

ON_MESSAGE(WM_MY_DELETEITEM,&CDDSServerDlg::OnDeleteItem)

4、定义函数OnDeleteItem

5.发消息

SendMessage或者PostMessage

CTreeItem* pItem ；

PostMessage(WM_MY_DELETEITEM,reinterpret_cast<WPARAM>((void*)(&pItem)),0);//pitem是指针，本来就是地址 不需要&

PostMessage(WM_MY_DELETEITEM,reinterpret_cast<WPARAM>((void*)(pItem)),0);

wparam和lparam工作：将需要传递的参数包装成一个结构体对象，在发出消息的时候把这个对象的地址作为参数。因此，在调用时处理函数就可以拿到这些数据了。微软怕一个地址不够用，所以给了两个作为备用。

参数转换为wparam：(WPARAM)((void*)&m_param)

wparam转换为参数类型： pItem = reinterpret_cast<CSuperGridCtrl::CTreeItem*>((void*)wParam);

## 三、自定义消息中如果需要定义WPARAM和LPARAM，该怎么使用和分配？
写Windows程序不可避免要使用自定义的消息，也就是从WM_USER开始定义的消息。在定义一个消息后，往往我们还要定义针对该消息的WPARAM甚至是LPARAM。WPARAM和LPARAM是什么，可以参考MSDN的SendMessage或者WindowProc。归结成一点就是对应消息的额外附带信息。

从我个人的经历来看，当定义了一个消息并且还需要定义额外的附加信息时，往往该附加信息会被先定义到WPARAM上。为什么？很简单，因为SendMessage(HWND, UINT, WPARAM, LPARAM)中，WPARAM排地比较前面，写起来方便。特别是用多了MFC后，CWnd::SendMessage中WPARAM和LPARAM都默认是0，定义在WPARAM上，代码都可以少些几个字符。

但是，我不得不说，有时候我还挺困惑一个问题的，就是WPARAM和LPARAM中的W和L到底指代什么？既然两个参数都附带信息，并且被标注成不同的名字，那么区别又在哪里？在实际使用中进行混用或者怎么用怎么方便是否合适？

今天，写这篇文章就是我觉得日子不能这么浑浑噩噩地过啊。。。这些看似简单的问题，也许不简单啊。所以google和bing了一把。这里就把找到的一些资料汇总一下。

WPARAM和LPARAM在现今看来，其实区别不是很大。至少从所占用的字节数来看是一样的。32位的程序里都是32位的，64位的程序里都是64位。他们在系统头文件里一般是这么定义的：

1 typedef UINT_PTR            WPARAM;
2 typedef LONG_PTR            LPARAM;
他们的不同主要还是在16位Windows系统里。在16位的Windows系统里，WPARAM是16位，W指代“word”，而LPARAM是32位的，L指代“long”。

那么在使用上，他们有什么区别呢？从我找到的资料来看，当初消息的设计时，WPARAM主要用来作为句柄和整数的载体，而LPARAM用来传递指针或者复合型的数据，比方说WM_CREATE和WM_MOVE。

现在，我个人觉得两者在使用上的区别应该不大。但是看了这些设计想法，以后我会考虑把指针这些额外数据定义到LPARAM上。

## 四、利用自定义消息处理函数的WPARAM或LPARAM参数传递指针
有自定义消息：
#define WM_TEST WM_USER+121
消息处理函数：
afx_msg void OnTest(WPARAM wParam,LPARAM lParam); 该消息是一个主线程向辅助线程发送数据的消息。
主线程中发送消息的代码：

```cpp
       m_param.pDoc=pDoc;
    m_param.pSpecAnlyz=m_pSpecAnlyz;
    
//    CWinThread* pThread=AfxBeginThread(ThreadTest,static_cast<LPVOID>(&m_param));
    m_pTestThread=(CTestThread*)AfxBeginThread(RUNTIME_CLASS(CTestThread));
    Sleep(200);
    // 必须先把&m_param转化为void*指针，然后才可以进一步强制转化为WPARAM
    m_pTestThread->PostThreadMessageW(WM_TEST,(WPARAM)((void*)&m_param),1);// 发送测试消息  
```
    m_param为自定义类型，其成员为指针变量，因此 m_param无法强制转化为WPARAM。但是m_param的地址指针和WPARAM一样，都是4个字节，因此可以将m_param的地址指针起那个只转换为WPARAM。在VC.NET2005中，直接转换编译报错。考虑到void * 之后，先将&m_param转化为void*指针，然后才可以进一步强制转化为WPARAM。

同样的，在OnTest中，必须先将wParam强制转换为void * ，然后进一步转换为其他类型。

PS：网上有高手指出，局部指针变量最好不要作为wParam或 lParam传递，因为当消息响应时该变量可能已经不在了。
