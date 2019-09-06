## [MFC——CTreeCtrl数据的处理：如何将数据和逻辑分开？](https://blog.csdn.net/iteye_21202/article/details/82546195)
CTreeCtrl是windows界面开发中经常用到，也是windows系统中经常见到的一种控件，比如文件夹目录，那就是一个典型的应用，不过，很多资料中，对于CTreeCtrl的数据的处理，存在一些问题。我们可以先看下面的代码：
```cpp
void CFileTreeCtrl::InitTreeCtrl(const CString& strDir, HTREEITEM hItem)
{
	CFileFind finder;
	CString strFindDir = strDir;
	strFindDir += _T("\\*.*");
	BOOL bWorking = finder.FindFile(strFindDir);
	while(bWorking)
	{
		HTREEITEM hItemTmp;
		bWorking = finder.FindNextFile();
		if(finder.IsDirectory() && !finder.IsDots())
		{
			CString szFileName = finder.GetFileName();
			CString strNextDir = finder.GetFilePath();
			SHFILEINFO sfi;
			SHGetFileInfo(strNextDir+_T("\\"), 0, &sfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX|SHGFI_DISPLAYNAME);
			TV_INSERTSTRUCT TCItem;
			ZeroMemory(&TCItem,sizeof(TV_INSERTSTRUCT));
			
			TCItem.hInsertAfter = TVI_LAST;
			TCItem.item.mask           = TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_STATE|TVIF_SELECTEDIMAGE;
			TCItem.item.pszText        = sfi.szDisplayName;
			TCItem.item.iImage         = sfi.iIcon;
			TCItem.item.iSelectedImage = sfi.iIcon;
			TCItem.item.lParam  = 0;
 
			if(hItem == NULL)
			{
				TCItem.hParent = m_hRoot;
				//hItemTmp = InsertItem(szFileName, m_hRoot);
				hItemTmp = InsertItem(&TCItem);
			}
			else
			{
				TCItem.hParent = hItem;
				//hItemTmp = InsertItem(szFileName, hItem);
				hItemTmp = InsertItem(&TCItem);
			}
			
			TCHAR* pData = new TCHAR[strNextDir.GetLength()];
			lstrcpy(pData, (LPCTSTR)strNextDir);
			SetItemData(hItemTmp, (DWORD_PTR)pData);
			InitTreeCtrl(strNextDir, hItemTmp);
		}
	}
	Expand(m_hRoot,TVE_EXPAND);
	finder.Close();
}
```
这种写法是比较常见的，这样写有什么问题呢？首先数据和逻辑混在一起；其次，每刷新一次，都要重新读取目录一次；再次，就是扩展性不好。这里是读取一个指定目录，那如果目录数据来源于数据库呢，又或者来源于服务程序？

所以，网上搜的资料，可以参考，可以借鉴，更要深入的思考和优化。

都提倡设计模式，但是说一套，做一套，并不是一个好的方式。有的人说起来一套套的，似乎这也会，那也懂，但是写的东西不是有内存泄露，就是导致程序异常。对于这种数据和逻辑混淆在一起的情况，其实大有存在。

先看看下面的代码：
```cpp
int CFileTreeCtrl::InitData(int parentId, HTREEITEM hItem)
{
	COprMySql OprMySql;
	BOOL bFlag=FALSE;
	TFile* temp;
 
	int nData = m_file[parentId];
	if (nData == 1)
	{
		Expand(hItem,TVE_EXPAND);
		temp = m_fileData[parentId];
		((CDlgUpFile*)(GetParent()))->m_EditPath.SetWindowText(temp->url);
		return 1;
	}
 
	vector<TFile> tFile;
	tFile.empty();
	vector<TFile>::iterator iter;
	
	char strtemp[20];
 
	OprMySql.QueryFileSqlByParentId(tFile, parentId);
 
	for(iter=tFile.begin(); iter!=tFile.end(); iter++)
	{
		temp = new TFile();
		*temp = *iter;
 
		TV_INSERTSTRUCT TCItem;
		ZeroMemory(&TCItem,sizeof(TV_INSERTSTRUCT));
			
		TCItem.hInsertAfter = TVI_LAST;
		TCItem.item.mask           = TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_STATE|TVIF_SELECTEDIMAGE;
		TCItem.item.pszText        = temp->name.GetBuffer();
		//TCItem.item.iImage         = sfi.iIcon;
		//TCItem.item.iSelectedImage = sfi.iIcon;
		TCItem.item.lParam  = temp->id;
 
		TCItem.hParent = hItem;
		InsertItem(&TCItem);
 
		m_fileData[temp->id] = temp;
 
	}
 
	OprMySql.Close();
 
	m_file[parentId] = 1;
 
	Expand(hItem,TVE_EXPAND);
 
	temp = m_fileData[parentId];
	((CDlgUpFile*)(GetParent()))->m_EditPath.SetWindowText(temp->url);
 
	return 0;
}
```
这段代码是从数据库获取目录信息，参数parentId，是表示选中的级数，一般默认加载一级的，当选中某个的时候，就在查询数据库获取它的子项，在这里做了一个标记，m_file[parentId]，第一次从数据库获取后，就设置m_file[parentId]=1，同时保存节点的信息m_fileData[temp->id] = temp。

而如果换成一个目录的话，只需要把这一句：OprMySql.QueryFileSqlByParentId(tFile, parentId);里的函数查询数据库，改成本地文件路径的数据。

这是初始的数据，当有增加和删除操作的时候，也一样。同时更新数据库和map的数据就好。

同样的道理，对于CListCtrl里的数据也是如此。
转载请注明原创链接：http://blog.csdn.net/wujunokay/article/details/13278131
