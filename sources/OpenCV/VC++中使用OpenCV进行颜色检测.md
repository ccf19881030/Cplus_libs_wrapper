# VC++中使用OpenCV进行颜色检测
在VC++中使用OpenCV进行颜色检测非常简单，首选读取一张彩色图像，并调用函数`cvtColor(img, imgHSV, COLOR_BGR2HSV);`函数将原图img转换成HSV图像imgHSV，再设置好HSV三个分量的上限和下限值，调用`inRange`函数`inRange(imgHSV, lower, upper, mask);`将HSV色彩图像转换成掩码图，掩码图中只有黑白二值图像，从而达到颜色检测的目的。颜色检测通常可以用于物体检测和跟踪中，尤其在不同的图像和物体中根据特定的颜色去筛选出某个物体。

## RGB色彩空间和HSV色彩空间
RGB色彩空间是一种被广泛接受的色彩空间，但是该色彩空间过于抽象，我们不能够直接通过其值感知具体的色彩。我们更习惯使用直观的方式来感知颜色，HSV色彩空间提供了这样的方式。通过HSV色彩空间，我们能够更加方便地通过色调、饱和度和亮度来感知颜色。

其实，除了HSV色彩空间，我们讨论的其他大多数色彩空间都不方便人们对颜色进行理解和解释。

## [HSL和HSV色彩空间](https://zh.wikipedia.org/wiki/HSL%E5%92%8CHSV%E8%89%B2%E5%BD%A9%E7%A9%BA%E9%97%B4)
HSV色彩空间从心理学和视觉的角度出发，提出人眼的色彩知觉主要包含三要素：

HSL和HSV都是将RGB色彩模型中的点表示在圆柱坐标系中的方法。这两种表示法试图做到比基于笛卡尔坐标系几何结构的RGB模型更加直观。

HSL即色相、饱和度、亮度（英语：Hue, Saturation, Lightness）。

HSV即色相、饱和度、明度（英语：Hue, Saturation, Value），又称HSB，其中B即英语：Brightness。

色相（Hue）是色彩的基本属性，就是平常所说的颜色名称，如红色、黄色等。
饱和度（Saturation）是指色彩的纯度，越高色彩越纯，低则逐渐变灰，取0-100%的数值。
明度（Value）、亮度（Lightness）、亮度（Brightness），取0-100%。
HSL和HSV二者都把颜色描述为在圆柱坐标系内的点，这个圆柱的中心轴底部为黑色，顶部为白色，而它们中间是灰色渐变，绕这个轴的角度对应于“色相”，到这个轴的距离对应于“饱和度”，而沿着这个轴的高度对应于“亮度”、“色调”或“明度”。

这两种表示在目的上类似，但在方法上有区别。二者在数学上都是圆柱，但HSV（色相、饱和度、明度）在概念上可以被认为是颜色的倒圆锥体（黑点在下顶点，白色在上底面圆心），HSL在概念上表示了一个双圆锥体和圆球体（白色在上顶点，黑色在下顶点，最大横切面的圆心是半程灰色）。注意尽管在HSL和HSV中“色相”指称相同的性质，它们的“饱和度”的定义是明显不同的。

因为HSL和HSV是设备依赖的RGB的简单变换，(h, s, l)或 (h, s, v)三元组定义的颜色依赖于所使用的特定红色、绿色和蓝色“加法原色”。每个独特的RGB设备都伴随着一个独特的HSL和HSV空间。但是 (h, s, l)或 (h, s, v)三元组在被约束于特定RGB空间比如sRGB的时候就更明确了。

HSV模型在1978年由埃尔维·雷·史密斯创立，它是三原色光模式的一种非线性变换，如果说RGB加色法是三维直角座标系，那么HSV模型就是球面座标系。

![HSL和HSV色彩空间](https://img-blog.csdnimg.cn/direct/7c01c6f59a6f417390a2d6140db4377b.png)
## HSV模型的用途
HSV模型通常用于计算机图形应用中。在用户必须选择一个颜色应用于特定图形元素各种应用环境中，经常使用HSV色轮。在其中，色相表示为圆环；可以使用一个独立的三角形来表示饱和度和明度。典型的，这个三角形的垂直轴指示饱和度，而水平轴表示明度。在这种方式下，选择颜色可以首先在圆环中选择色相，在从三角形中选择想要的饱和度和明度。
![HSV圆环和三角形表示法](https://img-blog.csdnimg.cn/direct/2bb2817e7cc0449b9c5c9676c63e7908.png)
**HSV色轮允许用户快速的选择众多颜色。**

HSV模型的另一种可视方法是圆锥体。在这种表示中，色相被表示为绕圆锥中心轴的角度，饱和度被表示为从圆锥的横截面的圆心到这个点的距离，明度被表示为从圆锥的横截面的圆心到顶点的距离。某些表示使用了六棱锥体。这种方法更适合在一个单一物体中展示这个HSV色彩空间；但是由于它的三维本质，它不适合在二维计算机界面中选择颜色。
![HSV圆锥体模型](https://img-blog.csdnimg.cn/direct/28efa658ffca4dab9f9570297e30788a.png)
**HSV模型的圆锥表示适合于在一个单一物体中展示整个HSV色彩空间**

HSV色彩空间还可以表示为类似于上述圆锥体的圆柱体，色相沿着圆柱体的外圆周变化，饱和度沿着从横截面的圆心的距离变化，明度沿着横截面到底面和顶面的距离而变化。这种表示可能被认为是HSV色彩空间的更精确的数学模型；但是在实际中可区分出的饱和度和色相的级别数目随着明度接近黑色而减少。此外计算机典型的用有限精度范围来存储RGB值；这约束了精度，再加上人类颜色感知的限制，使圆锥体表示在多数情况下更实用。


## VC++中使用OpenCV对图像进行颜色检测
通过学习油管博主[murtazahassan](https://www.youtube.com/@murtazasworkshop)的视频[Learn-OpenCV-cpp-in-4-Hours](https://youtu.be/2FYm3GOonhkOpenCV-Python-Tutorials-and-Projects)，里面第6个OpenCV示例将到如何从一副兰博基尼的轿车图像中进行颜色检测，相关代码地址为:[Learn-OpenCV-cpp-in-4-Hours
/Chapter6.cpp](https://github.com/murtazahassan/Learn-OpenCV-cpp-in-4-Hours/blob/main/Chapter6.cpp)
如下所示：
```cpp
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

///////////////  Color Detection  //////////////////////

void main() {

	string path = "Resources/lambo.png";
	Mat img = imread(path);	// 原图
	Mat imgHSV, mask;	// HSV图，掩码图
	int hmin = 0, smin = 110, vmin = 153;	// HSV三个分量的最小值
	int hmax = 19, smax = 240, vmax = 255;  // HSV三个分量的最大值

	cvtColor(img, imgHSV, COLOR_BGR2HSV);	// 将img原图转换为HSV色彩空间图像

	namedWindow("Trackbars", (640, 200));	// 创建一个宽为640，高为200的，窗口名称为Trackbars的窗口
	createTrackbar("Hue Min", "Trackbars", &hmin, 179); // 在窗口名称为Trackbars的窗口中创建一个名为Hue Min的滑动条，最小值默认为0，最大值为179，滑动条所在值即为hmin，用来控制H分量的最小值
	createTrackbar("Hue Max", "Trackbars", &hmax, 179); // 在窗口名称为Trackbars的窗口中创建一个名为Hue Max的滑动条，最小值默认为0，最大值为179，滑动条所在值即为hmax，用来控制H分量的最大值
	createTrackbar("Sat Min", "Trackbars", &smin, 255);	// 在窗口名称为Trackbars的窗口中创建一个名为Sat Min的滑动条，最小值默认为0，最大值为255，滑动条所在值即为smin，用来控制S分量的最小值
	createTrackbar("Sat Max", "Trackbars", &smax, 255); // 在窗口名称为Trackbars的窗口中创建一个名为Sat Max的滑动条，最小值默认为0，最大值为255，滑动条所在值即为smax，用来控制S分量的最大值
	createTrackbar("Val Min", "Trackbars", &vmin, 255);  // 在窗口名称为Trackbars的窗口中创建一个名为Val Min的滑动条，最小值默认为0，最大值为255，滑动条所在值即为vmin，用来控制V分量的最小值
	createTrackbar("Val Max", "Trackbars", &vmax, 255);  // 在窗口名称为Trackbars的窗口中创建一个名为Val Max的滑动条，最小值默认为0，最大值为255，滑动条所在值即为vmax，用来控制V分量的最大值

    // 循环检测用户操作，用户可以通过Trackbars窗口中的滑动条分别控制H、S、V三个分量的最小和最大值，从而控制mask图像的最终呈现
	while (true) {

		Scalar lower(hmin, smin, vmin);
		Scalar upper(hmax, smax, vmax);
		inRange(imgHSV, lower, upper, mask);	// 根据lower和upper以及imgHSV图像生成mask图像

		imshow("Image", img);	// 显示原图
		imshow("Image HSV", imgHSV);  // 显示HSV色彩图像
		imshow("Image Mask", mask);	  // 显示mask图像
		waitKey(1);	// 等待用户键盘输入，等待1毫秒
	}
}
```

图片资源下载地址为：[https://github.com/murtazahassan/Learn-OpenCV-cpp-in-4-Hours/tree/main/Resources](https://github.com/murtazahassan/Learn-OpenCV-cpp-in-4-Hours/tree/main/Resources)
![lambo.png](https://img-blog.csdnimg.cn/direct/e8a7372800fb424bbaac5f0331a231e5.png)
将代码拷贝到Visual Studio 2017中控制台程序中运行，并配置好OpenCV的头文件和库文件，运行结果如下图所示：
![运行结果](https://img-blog.csdnimg.cn/direct/3b2e53417b014ed8bcadccf47f9c94e4.png)
通过`Trackbars`窗口中的一些HSV相关滑动条可以调整H、S、V分量的最大和最小值，从而控制·Image Mask`窗口中的最终图像的显示。




## 参考资料
- [HSL和HSV色彩空间](https://zh.wikipedia.org/wiki/HSL%E5%92%8CHSV%E8%89%B2%E5%BD%A9%E7%A9%BA%E9%97%B4)
- [OpenCV—HSV色彩空间基础知识](https://blog.csdn.net/cyj972628089/article/details/106880398)
- [三分钟带你快速学习RGB、HSV和HSL颜色空间](https://zhuanlan.zhihu.com/p/67930839)
- [https://github.com/murtazahassan/Learn-OpenCV-cpp-in-4-Hours](https://github.com/murtazahassan/Learn-OpenCV-cpp-in-4-Hours)
- [Learn-OpenCV-in-3-hours](https://github.com/murtazahassan/Learn-OpenCV-in-3-hours)
- [https://github.com/murtazahassan](https://github.com/murtazahassan)
- [Learn-OpenCV-cpp-in-4-Hours  C++ Video](https://www.youtube.com/watch?v=2FYm3GOonhk)
- [Learn-OpenCV-in-3-hours Python Video](https://youtu.be/WQeoO7MI0Bs)
