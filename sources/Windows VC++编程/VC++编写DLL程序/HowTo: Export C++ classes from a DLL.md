## [CodeProject-HowTo: Export C++ classes from a DLL](https://www.codeproject.com/Articles/28969/HowTo-Export-C-classes-from-a-DLL)
* [示例XyzSample_src源代码下载](https://www.codeproject.com/KB/cpp/howto_export_cpp_classes/XyzSample_src.zip)
## [VC++导入导出类](https://www.cnblogs.com/hanford/p/6177883.html)
## [C++ DLL导出类 知识大全](https://www.cnblogs.com/lidabo/p/7121745.html)

### 动态链接库的使用有两种方式，一种是显式调用。一种是隐式调用。
#### (1).显式调用：使用LoadLibrary载入动态链接库、使用GetProcAddress获取某函数地址。
#### (2).隐式调用：可以使用#pragma comment(lib, “XX.lib”)的方式，也可以直接将XX.lib加入到工程中。
