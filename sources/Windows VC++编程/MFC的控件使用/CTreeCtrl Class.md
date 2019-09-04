## CTreeCtrl类
### [CTreeCtrl Class](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/ctreectrl-class?view=vs-2019)
### [使用 CTreeCtrl](https://docs.microsoft.com/zh-cn/cpp/mfc/using-ctreectrl?view=vs-2019)
### [MFC制作一个可自由添加删除节点的树（CTreeCtrl）类](https://blog.csdn.net/zhuzexujiaxing/article/details/70483893)
### [MFC Tree usage and example – MFC Tutorial X](http://www.codersource.net/2010/01/30/mfc-tree-usage-and-example/)
### [VC中CTreeCtrl控件单击事件,如何判断是单击的是某项的按钮,还是单击的文字](https://blog.csdn.net/qq_23992597/article/details/51007202)
```cpp
void CViewTree::OnClkTree(NMHDR* pNMHDR, LRESULT* pResult)
{
 
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	UINT uFlag = 0;
	HTREEITEM hCurSel = GetSelectedItem();
	HTREEITEM hItem = HitTest(pt, &uFlag);
	if(hItem)
	{
		if(hCurSel != hItem)
		{
			SelectItem(hItem);
		}
		CString strText(_T(""));
		if(TVHT_ONITEMBUTTON & uFlag)
		{
			strText.Format(_T("On Button"));
		}
		else if(TVHT_ONITEMLABEL & uFlag)
		{
			strText.Format(_T("On Label"));
		}
		else
		{
			strText.Format(_T("On Others"));
		}
		AfxMessageBox(strText);
	} 
 
// 	HTREEITEM hTreeItem = GetSelectedItem(); 
// 	//以GetItemText()函数为例： 
// 	CString S1 = GetItemText(hTreeItem); 
// 	AfxMessageBox("You have selected "+S1);
 
	*pResult = 0;
 
}
```
### [CTreeCtrl应用控件技巧-创建/编辑Edit/API的详细使用(代码详细介绍)](https://blog.csdn.net/fantuanxiaot/article/details/76147580)
### [获取CTreeCtrl所有结点的附加数据(ItemData)](https://blog.csdn.net/wennuan80/article/details/52839618)
```cpp
vector<DWORD_PTR> GetTreeCtrlAllNodeItemData(CTreeCtrl& _ctrl)
{
	vector<DWORD_PTR> lstItmData;
	HTREEITEM hItem=_ctrl.GetRootItem();
	DWORD_PTR itmData = NULL;
 
	if (NULL == hItem)
	{
		return lstItmData;
	}
	
	do
	{
		itmData = _ctrl.GetItemData(hItem);
		lstItmData.push_back(itmData);
A:
		if (_ctrl.ItemHasChildren(hItem))
		{
			hItem=_ctrl.GetChildItem(hItem);
			itmData = _ctrl.GetItemData(hItem);
			lstItmData.push_back(itmData);
			do
			{
				goto A;
			} while(hItem=_ctrl.GetNextSiblingItem(hItem));
		}
	} while(hItem=_ctrl.GetNextSiblingItem(hItem));
 
	return lstItmData;
}
```

## CTreeView 类
### [CTreeView 类](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/ctreeview-class?view=vs-2019)
### [CTreeCtrl 与CTreeView](https://docs.microsoft.com/zh-cn/cpp/mfc/ctreectrl-vs-ctreeview?view=vs-2019)
