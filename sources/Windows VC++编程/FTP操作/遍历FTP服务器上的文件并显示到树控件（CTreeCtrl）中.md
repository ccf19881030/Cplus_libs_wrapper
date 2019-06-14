## [遍历FTP服务器上的文件并显示到树控件（CTreeCtrl）中](http://www.voidcn.com/article/p-dkofiomx-uv.html)
```cpp
void CFtpUpLoadDlg::RefreshTree()
{
	CString server;
	GetDlgItemText(IDC_EDIP,server);
	m_TreeInfo.DeleteAllItems();
	hRoot =  m_TreeInfo.InsertItem(server,0,0);//根节点为FTP服务器ip
	ListDir("",hRoot);
}

void CFtpUpLoadDlg::ListDir(CString dir, HTREEITEM hParent)
{
	CString filename ;
	CInternetSession session;
	CFtpConnection* pTemp  =  session.GetFtpConnection(strip,strusr,strpwd,atoi(strport));
	CFtpFileFind Find(pTemp);

	HTREEITEM hItem = hParent;
	HTREEITEM hSubItem;

	BOOL ret ;
	ret = Find.FindFile(dir,INTERNET_FLAG_EXISTING_CONNECT);

	while (ret)
	{
		ret = Find.FindNextFile();
		if (Find.IsDots())// 遍历到.或是..
		{
			continue;
		}else if (Find.IsDirectory())
		{
			filename = Find.GetFileName();
			hSubItem = m_TreeInfo.InsertItem(filename,0,0, hParent);//将遍历到的文件夹加入到树控件
			
			ListDir(dir+"\\"+filename,hSubItem);	
		}else{
			filename = Find.GetFileName();
			hSubItem = m_TreeInfo.InsertItem(filename,0,0, hParent);//将遍历到的文件加入到树控件
		}
	}
	Find.Close();
	delete pTemp;
}
```
