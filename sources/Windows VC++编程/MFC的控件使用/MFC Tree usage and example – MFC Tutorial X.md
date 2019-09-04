## [MFC Tree usage and example – MFC Tutorial X](http://www.codersource.net/2010/01/30/mfc-tree-usage-and-example/)
MFC Tree controls are one of the useful controls to show a hierarchical view of items. They can show the folders list, any parent – child relationship items etc.,

CTreeCtrl is the class which handles all the MFC Tree Control related operations. It has all the necessary methods to Add MFC Tree control items, Retrieve selected items, Remove the items and other event handlers needed.

This article explains how to Initialize a MFC Tree control, Add items to Tree control, Retrieve items, Delete items and one event, Selection Change.
MFC Tree Control – CTreeCtrl Initialization:

These are the necessary steps to initialize a Tree Control in MFC.

Add a Tree control Resource to the dialog box in the Resource Assign a Resource ID to the tree control using the properties dialog.
Add a CTreeCtrl type member variable to the Tree Control using Class Wizard. (To do this, press Ctrl + W keys together to invoke the class wizard. In Member Variables Tab, select the tree control’s ID and press Add member. Variables can be created now). Let us assume the variable name to be m_MFC_Tree
MFC Tree Control – Adding Items:

For adding items in Tree control, the function InsertItem is the one to be used. This returns a handle to the tree item type as HTREEITEM. Look at the following samples.
Adding item to the root:
```cpp
HTREEITEM hParent = m_MFC_Tree.InsertItem("ItemText",TVI_ROOT);
```
The InsertItem function returns a handle to HTREEITEM, which can be used for adding items down the levels. This function InsertItem is overloaded and has some more signatures. But for a simple insert of items, this function signature is more than enough for us.
Adding child items to the Parent:

This is a very simple operation. We need the parent handle of type HTREEITEM for creating the child item.
```cpp
HTREEITEM hParent = m_MFC_Tree.InsertItem("ItemText",TVI_ROOT);
HTREEITEM hChild = m_MFC_Tree.InsertItem("Child ItemText",hParent,TVI_LAST);
```
Usually adding child items in such Tree controls will use recursive functions. For example if the requirement is to list all the folders under C: drive, it has to go through each folder to find the sub-folders. Recursive functions will handle such requests easily. A good example will be Windows Explorer.

If there are no “+” or “lines” found in the tree, change the Has Buttons and Has Lines properties of the Tree Control resource using the properties dialog box.
MFC Tree Control – Retrieving Selected Items:

There are two steps to this operation. We need to find the handle for the selected item and then retrieve the text for the corresponding item.
```cpp
HTREEITEM hItem = m_MFC_Tree.GetSelectedItem();

CString strItemText = m_MFC_Tree.GetItemText(hItem);
```
MFC Tree Control – Deleting Selected Items:

This is also very similar to the select operation. First we find the handle of the selected item to be deleted and then delete the item.
```cpp
HTREEITEM hItem = m_MFC_Tree.GetSelectedItem();
m_MFC_Tree.DeleteItem(hItem);
```
MFC Tree Control Event – Selection Changed:

To Add a handler for Selection Change:

    Open the class wizard
    Go to the Message maps tab
    Select the Tree control on the left side object IDs list box and select TVN_SELCHANGED on the right Messages List box.
    Click Add function and go to the function.
    
The following sample shows a Message box whenever there
occurs a selection change event in the Tree control.
```cpp
void CCoderSourceDlg::OnSelchangedTreectrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	// TODO: Add your control 	notification handler code here
	HTREEITEM hItem = 	m_MFC_Tree.GetSelectedItem();
	CString strItemText = 	m_MFC_Tree.GetItemText(hItem);

	MessageBox(strItemText);

	*pResult = 0;
}
```
Though this article does not cover every thing on the tree control, this should be enough to do some basic level of programming with MFC Tree control. 
