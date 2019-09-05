## [MFC EDIT控件 接收“回车”与“ESC”键 退出问题！](https://blog.csdn.net/zhanglifu3601881/article/details/80614185)
在做MFC程序开发过程中，经常会出现DEIT控制按下了ENTER或者ESC键后，程序就会立刻退出（这是MFCR ）实现的一种机制，但往往并不是我们想要的），今天教大家两种方法解决。  
以下是简单的代码示例： 
   
###【方法1】  可以先重载OnOK函数 
```cpp
  voidCTestDlg::OnOK()  
  {     //里面什么也不写}  
```
   
  然后重载PreTranslateMessage函数  
  把ESC键的消息，用RETURN键的消息替换，这样，按ESC的时候，也会执行刚才的OnOK函数，这样问题就可以解决了。  
   ```cpp
  BOOL   CxxxDlg::PreTranslateMessage(MSG*   pMsg)    
  {  
    if(pMsg->message==WM_KEYDOWN   &&   pMsg->wParam==VK_ESCAPE)    
    {    
    pMsg->wParam=VK_RETURN;   //将ESC键的消息替换为回车键的消息，这样，按ESC的时候  
                                                                                      //也会去调用OnOK函数，而OnOK什么也不做，这样ESC也被屏蔽  
      }    
    return   CDialog::PreTranslateMessage(pMsg);  
   
  }  
  ```
### 【方法2】  直接在重载的PreTranslateMessage函数中屏蔽回车和ESC的消息，和以上方法大同小异：  

在.h文件中加入  
```cpp
virtual BOOL PreTranslateMessage(MSG* pMsg);
```
 在.cpp文件中实现如下代码即可！
```cpp
  BOOL BaseOperation::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;


	return CDialog::PreTranslateMessage(pMsg);
}
```


建议直接用方法2操作，对了，如果程序中有多个对话框或都窗口，最好是对每一个对话框及窗口均重载PreTranslateMessage方法
