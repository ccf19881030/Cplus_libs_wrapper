## 1、[如何重载MFC对话框的OnInitDialog()函数](https://blog.csdn.net/li_canhui/article/details/6636896)
在VS2010中，重载对话框的OnInitDialog()函数时，需要打开Class Wizard对话框，选择工程，选择类，然后在Virtual Functions标签中搜索OnInitDialog函数，
找到后点击右侧的Add Function按钮，即可为对话框重载OnInitDialog()函数。
## 2、[MFC对话框程序中的OnCreate和OnInitDialog函数](https://blog.csdn.net/kailee_hust/article/details/48227693)
最近做一个简单的项目，需要使用到MFC。虽说这个东西已经很老旧了，但是为了项目使用，开始学它。需要将一个osg三维文件显示在控件中，这里使用编辑框空间，调试过程中遇到了CEdit* e = (CEdit*)GetDlgItem(IDC_EDIT1);其中e的值为NULL，也就是说没有get到控件。

        遂去图书馆，翻书找到问题所在由于将GetDlgItem放在了OnCreate中，而OnCreate是在窗口被创建时响应发送的消息，此时对话框尚未创建完成，所以其中的控件还不存在，当然就错了，它可以用来设置窗口的属性。此时不应将取得控件等与控件相关的语句放在OnCreate中。而对话框的OnInitDialog成员函数是对话框在被创建激活后调用。,此时各种控件已经被创建，因此与控件相关的操作应该放在这里面。
