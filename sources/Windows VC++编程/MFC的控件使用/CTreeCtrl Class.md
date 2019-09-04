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
### [CTreeCtrl树控件：如何给MFC中的CTreeCtrl树控件添加右键菜单及使用](https://blog.csdn.net/m0_37251750/article/details/81235628)
#### 1、头文件中：
```cpp
// 可以用来屏蔽操作的消息
virtual BOOL PreTranslateMessage(MSG* pMsg);

// 右击树控件的节点弹出菜单
afx_msg void OnRclickTreeObjects(NMHDR* pNMHDR, LRESULT* pResult);

//展开树控件某一结点下的所有子节点（递归函数）
void ExpandAllTreeItem(HTREEITEM hTreeItem);

// 获取树节点的等级
int GetItemLevel(HTREEITEM hItem);

CTreeCtrl m_objectsTreeCtrl;
```
#### 2、源文件中：
```cpp
#define  IDM_TREEUPDATE 10001
#define IDM_TREEDELETE 10002

BEGIN_MESSAGE_MAP(CObjectsDialog, CAcUiDialog)
    //{{AFX_MSG_MAP(CObjectsDialog)
    ON_WM_SIZE()
    ON_WM_TIMER()
    ON_NOTIFY(NM_RCLICK, IDC_TREE_OBJECTS, &CObjectsDialog::OnRclickTreeObjects)
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_OBJECTS, &CObjectsDialog::OnTvnSelchangedObjectsTree)
    ON_COMMAND(IDM_TREEUPDATE, &CObjectsDialog::OnUpdataSection)
    ON_COMMAND(IDM_TREEDELETE, &CObjectsDialog::OnDeleteSection)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CObjectsDialog::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN)
    {
        // 屏蔽ESC和回车键
        if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
        {
            return TRUE;
        }
    }

    return CAcUiDialog::PreTranslateMessage(pMsg);
}

BOOL CObjectsDialog::OnInitDialog()
{
    CAcUiDialog::OnInitDialog();
    m_objectsTreeCtrl.ModifyStyle(NULL, TVS_HASLINES | TVS_LINESATROOT);
    CString strItemTunnel = _T("根节点");
    HTREEITEM htItem = m_objectsTreeCtrl.InsertItem(strItemTunnel, TVI_ROOT, TVI_FIRST);
    CString strTunnel = _T("节点1");
    HTREEITEM htRoomChild = m_objectsTreeCtrl.InsertItem(strTunnel, htItem);

    ExpandAllTreeItem(htItem );

    return TRUE;
}

void CObjectsDialog::OnRclickTreeObjects(NMHDR* pNMHDR, LRESULT* pResult) 
{
    // TODO: Add your control notification handler code here
    if (curDoc() == NULL)
    {
        return;
    }

    CPoint point;
    GetCursorPos(&point);
    CPoint pointInTree = point;
    m_objectsTreeCtrl.ScreenToClient(&pointInTree);

    HTREEITEM item;
    UINT flag = TVHT_ONITEM;
    item = m_objectsTreeCtrl.HitTest(pointInTree, &flag);

    if(item == NULL)
    {
        return;
    }

    m_objectsTreeCtrl.SelectItem(item);

    int nLevel = GetItemLevel(item);
    CString strItemName = m_objectsTreeCtrl.GetItemText(item);
    if (nLevel == 1)
    {
        CMenu menu;
        menu.CreatePopupMenu();
        menu.AppendMenu(MF_STRING, IDM_TREEUPDATE, _T("更新大样图"));
        menu.AppendMenu(MF_STRING, IDM_TREEDELETE, _T("删除大样图"));
        menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this, NULL);
        menu.DestroyMenu();
    }
    else if (nLevel == 2)
    {
        CMenu menu;
        menu.CreatePopupMenu();
        menu.AppendMenu(MF_STRING, IDM_TREEUPDATE, _T("更新大样图"));
        menu.AppendMenu(MF_STRING, IDM_TREEDELETE, _T("删除大样图"));
        menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this, NULL);
        menu.DestroyMenu();
    }
    else
    {
        return;
    }

    *pResult = 1;
}

void CObjectsDialog::OnUpdataSection()
{
    HTREEITEM hItem = m_objectsTreeCtrl.GetSelectedItem();
    if (!hItem)
    {
        return;
    }

    int nLevel = GetItemLevel(hItem);

    if (1 == nLevel)
    {

    }
}

void CObjectsDialog::ExpandAllTreeItem(HTREEITEM hTreeItem)
{
    if(!m_objectsTreeCtrl.ItemHasChildren(hTreeItem))
    {
        return;
    }
    //若树控件的根节点有子节点则获取根节点的子节点
    HTREEITEM hNextItem = m_objectsTreeCtrl.GetChildItem(hTreeItem);

    while (hNextItem)
    {
        //递归，展开子节点下的所有子节点
        ExpandAllTreeItem(hNextItem);
        hNextItem = m_objectsTreeCtrl.GetNextItem(hNextItem, TVGN_NEXT);
    }
    m_objectsTreeCtrl.Expand(hTreeItem,TVE_EXPAND);
}

int CObjectsDialog::GetItemLevel(HTREEITEM hItem)
{
    int nIndex = 0;
    while (hItem)
    {
        hItem = m_objectsTreeCtrl.GetParentItem(hItem);
        nIndex++;
    }
    return nIndex;
}

// 为树节点绑定数据和获取数据
HTREEITEM htRoomChild = m_objectsTreeCtrl.InsertItem(strTunnel, 1, 1, htItem);
LONG_PTR tunnelId = idTunnel.asOldId();
m_objectsTreeCtrl.SetItemData(htRoomChild, tunnelId);

LONG_PTR objId = m_objectsTreeCtrl.GetItemData(htRoomChild );
AcDbObjectId entId;
entId.setFromOldId(objId);
if (entId.isValid())
{
    aryEntityId.append(entId);
}
```

### [MFC中CTreeCtrl右击选中节点并弹出右击菜单](https://blog.csdn.net/lvecc/article/details/20461019)
1.在父窗口响应NM_RCLICK消息
2.在NM_RCLICK消息响应函数中添加：
```cpp
void CDlgIndicatorsTree::OnRclickTreeindicators(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;
    CPoint pt   ;//用于获取CTreeCtrl右击点在CTreeCtrl的坐标 这里主要因为CTreeCtrl的点击测试的坐标点是基于自身坐标系 （HitTest为CTreeCtrl的成员函数）

    CPoint ptSc ;//右击菜单的右上角的位置是基于屏幕坐标系  
    UINT  flag;
    GetCursorPos(&pt); //获取当前点击坐标的全局坐标 
    ptSc =  pt;
    ScreenToClient(&pt);

    MapWindowPoints(&m_treeIndicators,&pt,1);//MapWindowPoint  为父类（CDialog）的成员函数,  将坐标系映射为CTreeCtrl的坐标系

    HTREEITEM hItem  = m_treeIndicators.HitTest(pt,&flag) ;
    if(NULL != hItem){
        m_treeIndicators.Select(hItem,TVGN_CARET);//设置点击节点为当前选中节点
        CMenu m,*mn;
        m.LoadMenu(IDR_INDIEDIT);//加载菜单资源
        mn = m.GetSubMenu(0);//获取菜单子项
        mn->TrackPopupMenu(TPM_LEFTALIGN,ptSc.x,ptSc.y,this);    //显示菜单
    }
}
```
## CTreeView 类
### [CTreeView 类](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/ctreeview-class?view=vs-2019)
### [CTreeCtrl 与CTreeView](https://docs.microsoft.com/zh-cn/cpp/mfc/ctreectrl-vs-ctreeview?view=vs-2019)
