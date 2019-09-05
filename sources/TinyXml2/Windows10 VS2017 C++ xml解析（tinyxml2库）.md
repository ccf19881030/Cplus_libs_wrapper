## [Windows10 VS2017 C++ xml解析（tinyxml2库）](https://blog.csdn.net/Jailman/article/details/85264849)
首先下载tinyxml2 7.0.1库：
[https://github.com/leethomason/tinyxml2/releases](https://github.com/leethomason/tinyxml2/releases)
打开tinyxml2，然后升级sdk，解决方案->重定解决方案目标，升级。
然后编译生成dll和库文件，在tinyxml2\Debug-Dll下，将tinyxml2.lib和tinyxml2.dll拷贝到新建的工程目录，在新建工程根目录新建include文件夹，将tinyxml2.h拷入，并将工程->配置属性->vc++目录->包含目录指向此目录。
编码：
```cpp
#include <iostream>
#include <tinyxml2.h>

#pragma comment(lib, "tinyxml2.lib")

using namespace std;


int main() {

	static const char* xml =
		"<?xml version=\"1.0\" encoding=\"utf-8\" ?>"
		"<entries>"
		"<entry name=\"My First Post\" age=\"52\">I believe every human has a finite number of heartbeats. I don't intend to waste any of mine</entry>"
		"<entry name=\"The Second\" age=\"\">You know, being a test pilot isn't always the healthiest business in the world.</entry>"
		"<entry>Entry</entry>"
		"<entry name=\"The Third\" secretdata=\"9809832\">We have an infinite amount to learn both from nature and from each other</entry>"
		"<entry name=\"Final Post...\" hidden=\"true\" age=\"3\">Across the sea of space, the stars are other suns.</entry>"
		"</entries>";

	tinyxml2::XMLDocument doc;
	doc.Parse(xml);

	tinyxml2::XMLHandle docHandle(&doc);

	tinyxml2::XMLElement *entry = docHandle.FirstChildElement("entries").ToElement();

	if (entry) {
		for (tinyxml2::XMLNode *node = entry->FirstChildElement(); node; node = node->NextSibling()) {
			tinyxml2::XMLElement *e = node->ToElement();

			const char *name = e->Attribute("name");
			if (name) cout << name << ": ";

			cout << e->GetText();

			int true_age = e->IntAttribute("age") + 50;

			cout << " " << true_age << endl;
		}
	}
	return 0;
}

```
