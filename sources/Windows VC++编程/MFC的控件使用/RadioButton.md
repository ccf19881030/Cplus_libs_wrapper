# 如何让Radio1或者Radio2默认选上？
## 方法1：
((CButton *)GetDlgItem(IDC_RADIO_HEX_SEND))->SetCheck(TRUE);	// 选上
((CButton *)GetDlgItem(IDC_RADIO_HEX_RECV))->SetCheck(TRUE);
## 方法2：关联一个congtrol型变量（子类化）
m_ctrlRadioSend.SetCheck(TRUE);
m_ctrlRadioRecv.SetCheck(TRUE);
## 方法3：关联一个int型变量（同样需要先分组）m_nRadio1
m_nRadio1别赋值-1表示哪个都没有选上。如果你把-1改成0，就会发现Radio1默认被选上了，依此类推，m_nRadio1的值为1就是第二个被选上了
获取状态很简单，UpdateData(TRUE)后判断m_nRadio1的值即可
