## [VC++ FTP文件遍历远程目录- www.codesc.net 编程代码分享](https://www.douban.com/note/697483808/)
```cpp
HCURSOR CFtpFileDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
void CFtpFileDlg::OnEnChangeEdit1()
{
	return;
}
void CFtpFileDlg::OnBnClickedButtonList()
{
	m_pInetSession=new CInternetSession(AfxGetAppName(),
		1,PRE_CONFIG_INTERNET_ACCESS);
	UpdateData(TRUE);
	try
	{
		m_pFtpConnection=m_pInetSession->GetFtpConnection(m_strHost,
			m_strUserName,m_strPassword,m_nPort);
		if(m_pFtpConnection != NULL)
		{
			CString m_strMsg;
			m_strMsg="连接成功\r\n";
		    m_strFtpInfo +=m_strMsg;
		    UpdateData(FALSE);
			FileList();
     	}
	}
	catch(CInternetException * pEx)
	{
		CString m_strMsg;
		m_strMsg="连接no成功\r\n";
		m_strFtpInfo +=m_strMsg;
		UpdateData(FALSE);
		TCHAR szError[1024];
		if(pEx->GetErrorMessage(szError,1024))
		{
			m_strMsg=(CString)szError;
			m_strFtpInfo +=m_strMsg;
			UpdateData(FALSE);
		}
		else
			AfxMessageBox("There was an exception");
		pEx->Delete();
		m_pFtpConnection=NULL;
	}
}
void CFtpFileDlg::OnDestroy()
{
	CDialog::OnDestroy();
}
void CFtpFileDlg::FileList()
{
	CString m_strMsg;
	CString m_strMsgAdd;//用来保存一些临时信息
	CStringArray m_saDir;//用数组来保存目录
	CFtpFileFind   finder(m_pFtpConnection);
    BOOL bWorking=finder.FindFile(_T("*"));
	while (bWorking)
    {
		bWorking = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		if (finder.IsDirectory())
		{
			m_saDir.Add(finder.GetFileName());//如果是目录的话，就保存在数组里
		}
		else
		{
			m_strMsg=finder.GetFileName()+"\r\n";//如果不是目录的话，就先显示出来
			for (int j=0;j<m_nIndex;j++)//这里就是来控制层次的
			{
				m_strMsgAdd="\t";
				m_strFtpInfo=m_strFtpInfo+m_strMsgAdd;
			}
			m_strFtpInfo +=m_strMsg;
			UpdateData(FALSE);
		}
	}
	finder.Close();	//连接关闭
	for(int i=0;i<m_saDir.GetSize();i++)//开始遍历目录里的文件
	{
		m_nIndex++;
		m_strMsg="["+m_saDir.GetAt(i)+"]"+"\r\n";//在目录的名字外面加上[]
		for(int j=1;j<m_nIndex;j++)
		{
			m_strMsgAdd="\t";
			m_strFtpInfo=m_strFtpInfo+m_strMsgAdd;
		}
		m_strFtpInfo +=m_strMsg;//先显示目录的名字
		UpdateData(FALSE);
		BOOL m_bSuc=FALSE;
		while (!m_bSuc)	//进入目录里面
		{
			m_bSuc=m_pFtpConnection->SetCurrentDirectory(m_saDir.GetAt(i));
		}
		FileList();	//递归调用!
		m_bSuc=FALSE;
		while(!m_bSuc)
		{
			m_bSuc= m_pFtpConnection->SetCurrentDirectory("..");
		}
		m_nIndex--;//控制层次
	}
}
```
