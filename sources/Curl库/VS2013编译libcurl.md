## [VS2013编译libcurl](https://www.cnblogs.com/foohack/p/4953156.html)
首先从这里下载libcurl的package：[http://curl.haxx.se/libcurl/](http://curl.haxx.se/libcurl/)

然后解压，里面有个winbuild文件夹，这里是为windows构建工程的文件夹。里面有nmake的makefile。调出VS2013的x86 Native Command prommpt。然后

cd 到winbuild目录下，
```shell
nmake /f Makefile.vc mode=static VC=12
```
还有很多编译选项可以传递，需要查阅，请用文本编辑器打开Makefile.vc自己看。可以编译动态库，静态库，产生PDB文件，也可以编译成Debug和Release版本。

 

最后就会在builds目录里面生成相应的bin include lib文件夹.

注：如果编译成XP，nmake中没有指定toolset=vc120_xp这类似物，所以需要设定链接参数：
```shell
/SUBSYSTEM:CONSOLE,5.01
```

 在nmake的makefilebuild.vc文件中找到链接参数，加上就可以了

### references:

[http://stackoverflow.com/questions/20171165/getting-libcurl-to-work-with-visual-studio-2013](http://stackoverflow.com/questions/20171165/getting-libcurl-to-work-with-visual-studio-2013)

[http://stackoverflow.com/questions/25740126/how-to-set-v120xp-in-cmake-with-vs2013s-nmake](http://stackoverflow.com/questions/25740126/how-to-set-v120xp-in-cmake-with-vs2013s-nmake)

[https://blogs.msdn.microsoft.com/vcblog/2012/10/08/windows-xp-targeting-with-c-in-visual-studio-2012/](https://blogs.msdn.microsoft.com/vcblog/2012/10/08/windows-xp-targeting-with-c-in-visual-studio-2012/)
