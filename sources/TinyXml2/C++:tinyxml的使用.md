## [C++:tinyxml的使用](https://www.cnblogs.com/whlook/p/7117306.html)
### 1. 简介
TinyXML2(最新版本)是一个开源的功能齐全的XML解析库 For C++，源码见：github。

### 2. 开始使用
首先从Github上获得源码，是一个完整的演示工程，我们只需要其中的tinyxml2.cpp和tinyxml2.h，将这两个文件拷贝到新建的VS工程目录下，然后就可以开始使用了，这是方法之一，也可以将源码编译为.lib然后链接到工程中。

然后包含头文件tinyxml2.h，并使用命名空间：using namespace tinyxml2

### 3. 写XML
```cpp
#include <iostream>
#include "tinyxml2.h"

using namespace std;

using namespace tinyxml2;

int main()
{
    // 新建一个空文档（表示完整的xml）
    XMLDocument xmlDoc;

    // 新节点
    XMLNode * pRoot = xmlDoc.NewElement("Root"); 

    // 插入到xmlDoc的第一个节点（根节点）
    xmlDoc.InsertFirstChild(pRoot); 

    // 新建一个元素
    XMLElement *pElement = xmlDoc.NewElement("IntValue"); 

    // 设置该元素（节点）的值
    pElement->SetText(10);

    // 设置该元素的属性（重载）
    pElement->SetAttribute("year", 2017);
    pElement->SetAttribute("key", "hello");

    // 将该节点添加到pRoot节点下("Root")
    pRoot->InsertEndChild(pElement);

    // 指向新的节点
    pElement = xmlDoc.NewElement("FloatValue");

    // 添加到pRoot节点（依次向下添加）
    pRoot->InsertEndChild(pElement);

    // 新建一个节点
    XMLElement *pNewElement = xmlDoc.NewElement("value1");
    
    // 设置节点的值
    pNewElement->SetText(1.0);

    // 将该节点添加到pElement节点下("FloatValue")
    pElement->InsertFirstChild(pNewElement);

    // 指向新的节点
    pNewElement = xmlDoc.NewElement("value2");

    // 设置节点的值
    pNewElement->SetText(2.0);

    // 将该节点插入到pElement节点下（依次向下添加)
    pElement->InsertEndChild(pNewElement);

    // 保存文件
    XMLError eResult = xmlDoc.SaveFile("test.xml");

    if (eResult != XML_SUCCESS)
        cout << "error\n";

    return 0;
}
```
结果 test.xml
```xml
<Root>
    <IntValue year="2017" key="hello">10</IntValue>
    <FloatValue>
        <value1>1</value1>
        <value2>2</value2>
    </FloatValue>
</Root>
```

### 4. 读取XML
```cpp
#include <iostream>
#include "tinyxml2.h"

using namespace std;

using namespace tinyxml2;

int main()
{
    // 新建一个空文档
    XMLDocument xmlDoc;

    // 读取指定的xml文件并判断读取是否成功
    XMLError eResult = xmlDoc.LoadFile("test.xml");
    if (eResult != XML_SUCCESS)
    {
        cout << "error\n";
        return XML_ERROR_FILE_NOT_FOUND;
    }

    // 获得该文件的第一个节点（根节点）
    XMLNode * pRoot = xmlDoc.FirstChild();
    if (pRoot == nullptr)
        return XML_ERROR_FILE_READ_ERROR;

    // 找到该节点中的名字为 "IntValue"的第一个子节点
    XMLElement * pElement = pRoot->FirstChildElement("IntValue");
    if (pElement == nullptr)
        return XML_ERROR_PARSING_ELEMENT;

    // 读取子节点的值
    int iOutInt;
    eResult = pElement->QueryIntText(&iOutInt);
    if (eResult != XML_SUCCESS)
        return XML_ERROR_PARSING;
    else
        cout << iOutInt << endl;

    // 读取子节点的属性
    int iOutYear;
    eResult = pElement->QueryIntAttribute("year", &iOutYear);
    if (eResult != XML_SUCCESS)
        return XML_ERROR_PARSING;
    else
        cout << iOutYear << endl;

    // 获得该子节点的下一个兄弟节点（更深层的节点搜索类似）
    XMLElement * nextNode = pElement->NextSiblingElement();
    cout << nextNode->Name() << endl;

    return 0;
}
```

运行结果：
```shell
10 
2017
FloatValue
```
