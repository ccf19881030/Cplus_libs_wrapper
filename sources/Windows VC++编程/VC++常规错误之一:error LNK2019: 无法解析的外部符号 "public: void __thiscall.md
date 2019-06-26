## [VC++常规错误之一:error LNK2019: 无法解析的外部符号 "public: void __thiscall](https://blog.csdn.net/greenapple_shan/article/details/45744215)
error LNK2019: 无法解析的外部符号 “public: void __thiscall

遇到这种情况原因有二个,简单点说： 
其一：某个方法只是在.h里面声明，没有在cpp里面实现。 
其二：声明和实现都有了，但是没有添加到vs的虚拟工作目录下面，这时候如果在其他.cpp引用的话就抱这个错

比如我新建项目导入别人的代码的时候，忘记在上图显示的虚拟目录下添加CSContentJsonDictionary.h和CSContentJsonDictionary.cpp 
他就会报错说：1>CCSReader.obj : error LNK2019: 无法解析的外部符号 “public: void __thiscall cs::CSJsonDictionary::initWithDescription(char const )” (?initWithDescription@CSJsonDictionary@cs@@QAEXPBD@Z)，该符号在函数 “public: class CocoGUI::CocoWidget __thiscall CocoGUI::CCSReader::widgetFromJsonFile(char const *)” (?widgetFromJsonFile@CCSReader@CocoGUI@@QAEPAVCocoWidget@2@PBD@Z) 中被引用 
因为我在CocoGUI::CCSReader::widgetFromJsonFile(char const )的方法里面引用了cs::CSJsonDictionary::initWithDescription(char const )这个方法。解决办法就是在虚拟目录单击右键添加.h和.cpp.添加完之后重新生成以下就没问题了。
