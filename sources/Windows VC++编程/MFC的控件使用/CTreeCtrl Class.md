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
### [mfc ctreectrl遍历所有节点](https://blog.csdn.net/XuePiaoFei1/article/details/47093635)
遍历节点，并取得节点文本：
其中  m_vecTreeNode是CDlgTree成员变量（vector<string>  m_vecTreeNode），将所有节点文本保存。
由于使用多字符集，所以可以
如果使用Unicode，参考[http://blog.csdn.net/xuepiaofei1/article/details/47093375](http://blog.csdn.net/xuepiaofei1/article/details/47093375)
```cpp
//传入的ht是根节点
void CDlgTree::TravelAll(HTREEITEM ht)
{
 
	string strTemp;
	CString cstr = m_wndTree.GetItemText(ht);
	strTemp = cstr.GetBuffer(0);
	HTREEITEM hCurItem = m_wndTree.GetChildItem(ht);
	
	m_vecTreeNode.push_back(strTemp);
 
	HTREEITEM hNextItem;
	while(hCurItem)
	{
		hNextItem = hCurItem;
		TravelAll(hNextItem);
		hCurItem = m_wndTree.GetNextSiblingItem(hCurItem);
	}
}
```

### [MFC树形控件CTreeCtrl使用方法、遍历、SetCheck无效、根据名称获取节点、获取选中节点等问题](https://blog.csdn.net/shenhch/article/details/78606033)
#### 1.新建树形控件 
（1）、直接在MFC工具中拖入控件，然后增加成员变量CTreeCtrl m_Tree; 
（2）、动态创建 
CTreeCtrl::Create
```cpp
    BOOL Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );
    返回值：如果初始化成功则返回非零值；否则返回0
    参数说明
    dwStyle：指定tree view控件的风格。可以对这个控件使用tree view控件风格的任意组合。
    rect：   指定tree view控件的尺寸和位置。此参数可以是一个CRect对象或一个RECT结构。
    pParentWnd：指定tree view控件的父窗口，通常是一个CDialog。它不能是NULL。
    nID：指定tree view控件的ID。
   风格说明：
      TVS_HASLINES tree view控件的子项与它们的父项之间用线连接。
      TVS_LINESATROOT tree view控件用线连接子项和根项。
      TVS_HASBUTTONS tree view在每一个父项的左边添加一个按钮。
      TVS_EDITLABELS tree view控件允许用户编辑tree view项的标签。
      TVS_SHOWSELALWAYS 当tree view失去焦点时，使被选择的项仍然保持被选择
      TVS_DISABLEDRAGDROP 该tree view控件被禁止发送TVN_BEGINDRAG通知消息。
      TVS_NOTOOLTIPS tree view控件使用工具提示。
      TVS_SINGLEEXPAND 当使用这个风格时，改变在tree view中的选择将导致正被选择的项展开，而没有被选择的项收缩。如果用鼠标单击被选择的项，并且该项是关闭的，则该项就会展开。如果该被选择的项被单击时是打开的，则它就会收缩。
```
#### 2、插入节点
```cpp
    HTREEITEM hRoot;     // 树的根节点的句柄   
    HTREEITEM hCataItem; // 可表示任一分类节点的句柄
    hRoot = m_Tree.InsertItem(_T("Root"),0,0);
    for(int i=0; i < 5; i++)
    {
         CString str;
         str.Format(_T("Item%d"),i+1);
         hCataItem = m_Tree.InsertItem(str, 1, 1, hRoot, TVI_LAST);         
    }   
```
#### 3、获取选中节点名称
```cpp
void GetCheckNode( HTREEITEM hRoot )
{
    // 如果不是叶子节点
    if(TRUE == m_Tree.ItemHasChildren(hRoot))
    {
        if(TRUE == m_Tree.GetCheck(hRoot))
        {
            // 插入所有页节点
            InsertAllLeaves(hRoot);
        }
        else
        {
            // 查询所有节点，递归
            HTREEITEM hChild = m_Tree.GetChildItem(hRoot);
            while(NULL != hChild)
            {
                GetCheckNode(hChild);
                hChild = m_Tree.GetNextSiblingItem(hChild);
            }
        }
    }
    else // 是叶子节点
    {
        // 被选中
        if(TRUE == m_tTree.GetCheck(hRoot))
        {
            CString well = m_Tree.GetItemText(hRoot);
        }
    }
}


void InsertAllLeaves( HTREEITEM hRoot )
{
    HTREEITEM hChild = m_Tree.GetChildItem(hRoot);
    while(NULL != hChild)
    {
        if(TRUE == m_Tree.ItemHasChildren(hChild))
        {
            InsertAllLeaves(hChild);
        }
        else
        {
            CString well = m_Tree.GetItemText(hChild);
        }
        hChild = m_Tree.GetNextSiblingItem(hChild);
    }
}
```
#### 4、根据名称查找节点
```cpp
HTREEITEM  finditem(HTREEITEM  item, CString strtext)   
{  
    HTREEITEM  hfind;  
    //空树，直接返回NULL
    if(item ==  NULL)  
        return  NULL;  
    //遍历查找
    while(item!=NULL)  
    {  
        //当前节点即所需查找节点
        if(m_Tree.GetItemText(item) == strtext)  
            return   item;  
        //查找当前节点的子节点
        if(m_Tree.ItemHasChildren(item))  
        {  
            item   =   m_Tree.GetChildItem(item); 
            //递归调用查找子节点下节点
            hfind   =   finditem(item,strtext); 
            if(hfind)  
            {  
                return  hfind;  
            }  
            else   //子节点中未发现所需节点，继续查找兄弟节点
            {
                item = m_Tree.GetNextSiblingItem(m_ShowObjectTree.GetParentItem(item));  
            }
        }  
        else{   //若无子节点，继续查找兄弟节点
            item = m_Tree.GetNextSiblingItem(item);  
        }  
    }  
    return item;  
}
```
### 5、展开所有节点 
```cpp
void CMenuCreatDlg::OnMENUITEMexpandtree() //展开所有节点  
{  
    // TODO: Add your command handler code here  
     MyExpandTree(m_tree.GetRootItem());   
}  
   
void CMenuCreatDlg::MyExpandTree(HTREEITEM hTreeItem)  
{  
    if(!m_tree.ItemHasChildren(hTreeItem))  
    {  
        return;  
    }  
    HTREEITEM hNextItem = m_tree.GetChildItem(hTreeItem);  
    while (hNextItem != NULL)  
    {  
        MyExpandTree(hNextItem);  
        hNextItem = m_tree.GetNextItem(hNextItem, TVGN_NEXT);  
    }  
    m_tree.Expand(hTreeItem,TVE_EXPAND);  
}  
```

## CTreeView 类
### [CTreeView 类](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/ctreeview-class?view=vs-2019)
### [CTreeCtrl 与CTreeView](https://docs.microsoft.com/zh-cn/cpp/mfc/ctreectrl-vs-ctreeview?view=vs-2019)
