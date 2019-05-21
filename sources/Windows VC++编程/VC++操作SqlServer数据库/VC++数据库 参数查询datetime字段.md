# [VC++数据库 参数查询datetime字段](http://blog.chinaunix.net/uid-25958655-id-3961305.html)
其实可以用SQL的Convert()函数或是CAST()函数.
下面是一个例子:
```cpp
//按钮的单击事件响应函数
void CAdo_ParameterDlg::OnSelect() 
{
    // TODO: Add your control notification handler code here
    _ConnectionPtr pConn;
    _RecordsetPtr pRecordset;
    _CommandPtr pCmd;
    _ParameterPtr Param1;

    CString cError=" 1";    
    try
    {
        pConn.CreateInstance("ADODB.Connection");
        //_bstr_t strConnect="Provier=SQLOLEDB.1;User ID=sa;Password=123456; Server=127.0.0.1\\SQLEXPRESS; Initial Catalog=test;Persist Security Info=true";//SQL Server 的连接字串
        _bstr_t strConnect="Provider=SQLOLEDB.1;Server=127.0.0.1\\SQLEXPRESS;Password=123456;Persist Security Info=true;User ID=sa;Initial Catalog=test";//注127.0.0.1\\SQLEXPRESS改成T75JLZX6ILTGGAL\\SQLEXPRESS的话是肯定可以
        //如果使用127.0.0.1\\SQLEXPRESS要确保sql 2005 Express的"服务和连接的外围应用配置器-->》展开SQLEXPRESS-》下面有个远程连接，选取本地连接和远程连接-》选择同时使用TCP/IP和name pipes,确定 

        pConn->Open(strConnect,"","",adModeUnknown);//连接数据库
        cError=" 2";

    }
    catch(_com_error e)
    {
        AfxMessageBox(e.Description()+cError);
        return;
    }
    
    try
    {
        pRecordset.CreateInstance("ADODB.Recordset");
        pCmd.CreateInstance("ADODB.Command");
        pCmd->ActiveConnection=pConn;
                //代码段3 //这样可以查询时间字段
        pCmd->ActiveConnection=pConn;
        CString sCmd;
        CTime time(1982,2,2,0,0,0);
        cError="3";
        //sCmd.Format("select * from student where birthday= CAST('%s' as datetime)",time.Format("%Y-%m-%d %H:%M:%S") );
        sCmd.Format("select * from student where birthday= CONVERT(varchar(100),'%s',120)",time.Format("%Y-%m-%d %H:%M:%S") );
        
        cError="4";
        pCmd->CommandText=_bstr_t(sCmd);
        
        //end of 代码段3

        //
        pRecordset->Open((IDispatch*)pCmd, vtMissing, adOpenStatic, adLockOptimistic, adCmdUnspecified);
        int i=0;
        while(!pRecordset->adoEOF)
        {
            m_ListCtrl.InsertItem(i,""); //刚开始忘了加入这一行,所以一直没有数据显示
            m_ListCtrl.SetItemText(i,0, (char *) (bstr_t)pRecordset->GetCollect("id"));//m_ListCtrl是一个listControl
            m_ListCtrl.SetItemText(i,1, (char *) (bstr_t)pRecordset->GetCollect("sname"));
            m_ListCtrl.SetItemText(i,2, (char *) (bstr_t)pRecordset->GetCollect("grade"));
            m_ListCtrl.SetItemText(i,3, (char *) (bstr_t)pRecordset->GetCollect("class"));
            m_ListC
            trl.SetItemText(i,4, (char *) (bstr_t)pRecordset->GetCollect("birthday"));
            
            i++;
            pRecordset->MoveNext();
        }

    }
    catch(_com_error e)
    {
        AfxMessageBox(e.Description()+cError);
        return;
    }
}
```
具体的整个实例在这里: 
[http://blog.chinaunix.net/uid-25958655-id-3961324.html](http://blog.chinaunix.net/uid-25958655-id-3961324.html)
