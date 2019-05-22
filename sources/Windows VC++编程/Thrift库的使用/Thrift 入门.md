## 参考博文：[Thrift 入门](https://www.cnblogs.com/lpxblog/p/5381676.html)
### 1、下载[https://thrift.apache.org/download](https://thrift.apache.org/download)或[Apache Thrift Archive](http://archive.apache.org/dist/thrift/)

下载编译好的release可执行文件：

Thrift compiler forWindows (thrift-0.9.2.exe)

 

### 2、安装
把thrift-0.9.2.exe改名为thrift.exe放到path环境变量中存在的目录下，如：C:\Windows
改名为thrift.exe不是必须的，但是最好这样，因为修改后的名字使用起来输入方便一点。

### 3、运行生成代码

切换到thrift文件所在的目录，执行如下命令
```shell
thrift -r --gen cpp tutorial.thrift
```
