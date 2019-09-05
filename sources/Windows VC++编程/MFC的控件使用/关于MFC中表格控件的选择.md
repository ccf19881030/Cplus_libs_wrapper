## [关于MFC中表格控件的选择](https://bbs.csdn.net/topics/390646220)
目前比较好的开源控件有以下两个：
MFC Grid control 2.27：
http://www.codeproject.com/Articles/8/MFC-Grid-control-2-27

Ultimate Grid：
http://www.codeproject.com/Articles/20183/The-Ultimate-Grid-Home-Page

这两个都支持VS2013,MFC Grid control 2.27是一个人写的，Ultimate Grid是很多人写的，文档比较详细。
我只编译运行过他们的Demo，请有经验的说说哪个更好用点

简单跟过Ultimate的源码，数据源就是用MFC的CArray来实现的。The Ultimate Grid comes with number of pre-built datasources (ADO, DAO, ODBC, etc) which allow you to easily connect to a database.这点比较喜欢。
MFCGrid支持一个虚模式，不保存特定的数据，在一个回调函数中来处理数据显示问题。

个人目前偏向Ultimate，毕竟以前是商业控件，文档也比较好。
自己也用MFCGrid写过小Demo, 用起来也比较舒服。

还请实际用过的指点指点。

计划做一个类似广联达或易达的工程造价软件，部门内部使用。我们比较特殊，这些通用软件不合适。
需要用到数据库，现在选的是firebird
需要其功能接近Excel，方便数据的操作，数据隐藏，排序和筛选。
Excel文件的导入导出，这个可以自己写


Ultimate Grid    还可以  

有比较齐全的 配套文档。

论坛有个人还翻译部分教程
 

http://blog.csdn.net/huangws/article/details/8678499

不过似乎有错误 。。。。。

如果你现在已经选择了  Ultimate Grid  ， 请问你在做ODBC的时候是否出了问题。

是否方便一起讨论  wenluderen@gmail.com
