## [mfc 遍历ftp服务器文件并显示树形列表中](https://blog.csdn.net/lqlblog/article/details/17515837)
当我们第一次在在编写基于ftp协议的客户端程序时，相信在ftp下会犯很多很烦的问题：
### 一.搞不清楚ftp下文件的路径究竟是怎么样的，比如要putFile()时，里面的路径应该写什么
               1.其实ftp下文件路径是:根目录就是“/”,其他的和正常文件目录一样
               2.在ftp下设置当前目录时即调用SetCurrentDirectory()函数时路径该怎么写。
                   比如说我有个文件夹"ss"在根目录下它下面还有文件"123"文件夹，此时我要查找"123"文件夹下的文件目录
                   那么路径应该写成"/ss/123"这样才能遍历123文件夹下的文件

### 二. 关于CFtpFileFind 类在遍历过程中的问题

              因为MFC中不允许同时存在两个CFtpFileFind类，所以在遍历某一层文件夹时，必须首先把子文件夹先存起来，

             本层遍历完之后然后必须关掉CFtpFileFind对象，然后在取出子文件夹进行递归遍历。具体详细看代码。

###三.关于在ftp服务器文件在本地系统中的图标获得问题

             相信如果是获取本地文件在系统中的图标，直接用SHGetFileInfo函数获取即可

             但是如果文件不再本地电脑上而在远程服务器上怎么办呢？

             在这里，我的做法是在本地系统下建立一个临时文件，然后得到这个临时文件图标即可，取得图标之后在将临时文件删除就行了。

             可能还有其他的做法，比如说通过读注册表得到相关后缀名对应的图标，但是我觉得还是这种建立临时文件的方法比较简单。
```cpp
map<HTREEITEM,string> m_filMap;//节点与路径映射
map<HTREEITEM, string> m_nameMap;//节点与文件名映射
map<HTREEITEM, int> m_fileLen;

//文件目录加入树形视图控件
void CFileTreeCtr::FileShow(HTREEITEM root, CString strDir)
{
	//文件夹列表集合
	CStringList list(100);
	//文件夹对应树里的节点队列
	queue<HTREEITEM> curRoot;
	//设置当前路径
	pConnect->SetCurrentDirectory(strDir);
	//查找器
	CFtpFileFind finder(pConnect);	
	//在设置路径下查找
	BOOL bWorking = finder.FindFile();
	HTREEITEM curHtr;
	SHFILEINFO fileInfo;
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		CString strName  = finder.GetFileName();
		CString bsePath;
		if(strName == "." || strName == "..")
			continue;
		//显示文件
		if(strDir == "/")//根目录
			bsePath = strDir + strName;
		else			//子文件目录
			bsePath = strDir + "/" + strName;
		CString tPath;
		CFile* tmpFile;
		//文件还是文件夹标志
		int isDir = 0;
		//文件夹
		if(finder.IsDirectory())
		{
			//创建临时文件夹
			CreateDirectory(m_logicDir + "\lql" + strName, NULL);
			tPath = m_logicDir + "\lql" + strName;
			isDir = 1;
			list.AddTail(strName);
			//curRoot.push(curHtr);
		}
		else
		{
			isDir = 0;
			//为获取文件系统图标建立临时文件
			tmpFile = new CFile(m_logicDir + "\lql"+strName, CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone);
			//临时文件路径
			tPath = tmpFile->GetFilePath();
		}
		
		//获取文件系统图标
		int re = SHGetFileInfo(tPath, 0, &fileInfo, sizeof(fileInfo), SHGFI_ICON | SHGFI_SMALLICON);
		//插入文件图标
		int imIndex = m_imlst.Add(fileInfo.hIcon);
		//插入节点
		curHtr = InsertItem(strName, imIndex, imIndex, root);
		//关掉临时文件
		if(isDir == 0)
		{
			m_filMap[curHtr] = bsePath;
			m_nameMap[curHtr] = strName;
			m_fileLen[curHtr] = finder.GetLength();

			tmpFile->Close();
			//移除临时文件
			tmpFile->Remove(tPath);
			//释放资源
			delete tmpFile;
		}
		//删掉临时文件夹
		if(isDir == 1)
		{
			RemoveDirectory(tPath);
			//获取信息后插入节点
			curRoot.push(curHtr);
		}
	}
	//关掉本层下的目录(必须)
	finder.Close();
	//开始遍历子文件夹下文件
	while(!list.IsEmpty() && !curRoot.empty())
	{
		//strDir = list.RemoveHead();
		//当前文件的路径
		CString tmpPath;
		if(strDir == "/")
		{
			tmpPath = strDir + list.RemoveHead();
		}
		else
		{
			tmpPath = strDir + "/" + list.RemoveHead();
		}
		//当前文件所对应的节点
		HTREEITEM tmRoot = curRoot.front(); curRoot.pop();
		//递归子文件夹
		FileShow(tmRoot, tmpPath);
	}
｝
```
