1.传给未分配内存的const char* （LPCTSTR）指针. 
CString cstr(asdd);
const char* ch = (LPCTSTR)cstr;
ch指向的地址和cstr相同。但由于使用const保证ch不会修改，所以安全.

2.传给未分配内存的指针.
CString cstr = "ASDDSD";
char *ch = cstr.GetBuffer(cstr1.GetLength() + 1);
cstr.ReleaseBuffer();
//修改ch指向的值等于修改cstr里面的值.
//PS:用完ch后,不用delete ch,因为这样会破坏cstr内部空间,容易造成程序崩溃.

3.第二种用法。把CString 值赋给已分配内存的char *。
CString cstr1 = "ASDDSD";
int strLength = cstr1.GetLength() + 1;
char *pValue = new char[strLength];
strncpy(pValue, cstr1, strLength);

4.第三种用法.把CString 值赋给已分配内存char[]数组.
CString cstr2 = "ASDDSD";
int strLength1 = cstr1.GetLength() + 1;
char chArray[100];
memset(chArray,0, sizeof(bool) * 100); //将数组的垃圾内容清空.
strncpy(chArray, cstr1, strLength1);

[http://hi.baidu.com/net_speed_slow/item/b08cf6b0b6c98a941846975e](http://hi.baidu.com/net_speed_slow/item/b08cf6b0b6c98a941846975e)
[http://zhidao.baidu.com/question/94466457.html](http://zhidao.baidu.com/question/94466457.html)

CString转char[]:
[http://bbs.csdn.net/topics/70472781](http://bbs.csdn.net/topics/70472781)
