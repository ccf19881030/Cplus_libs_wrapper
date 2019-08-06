## [MFC中OnClose和OnDestroy的调用](https://blog.csdn.net/u014350471/article/details/38437287)
### 关于OnClose和OnDestroy:
* OnClose()：消息响应函数，响应WM_CLOSE消息，当"关闭"按钮被单击的时候（而不是我们眼睛所见的对话框从屏幕上消失时），该函数被调用。

* OnDestroy()：消息响应函数，响应WM_DESTROY消息，当一个窗口即将被销毁时，被发送。

### 在单视图程序中，程序退出时执行的操作顺序为：
* (1)用户点击退出按钮，发送了WM_CLOSE消息；
* (2)在WM_CLOSE消息的处理函数中，调用DestroyWindow()；
* (3)在DestroyWindow()中发送了WM_DESTROY消息；
* (4)在WM_DESTROY消息中调用PostQuitMessage()，发送WM_QUIT消息，结束消息循环。

### 总结
综上，程序先调用OnClose()（也可能不调用），然后调用OnDestroy()（必调用），所以，如果要进行程序结束时的清理工作，应该在OnDestroy（）中，
而不是在OnClose()，否则就有可能会出现内存泄漏的危险了！
