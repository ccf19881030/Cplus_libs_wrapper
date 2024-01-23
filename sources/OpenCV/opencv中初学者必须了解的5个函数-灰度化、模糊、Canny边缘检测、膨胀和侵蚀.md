# [opencv](https://opencv.org/)中初学者必须了解的5个函数-灰度化、模糊、边缘检测、膨胀和侵蚀
使用[OpenCV](https://opencv.org/)可以对彩色原始图像进行基本的处理，涉及到5个常用的处理：
- 1. 灰度化
- 2. 模糊处理
- 3. Canny边缘检测
- 4. 膨胀
- 5. 腐蚀
原图`test.png`如下所示：
![test.png](https://img-blog.csdnimg.cn/direct/e27028ef9e2b497ba72163aa3176faa4.png#pic_center)
可以直接去[https://github.com/murtazahassan/Learn-OpenCV-cpp-in-4-Hours/blob/main/Resources/test.png](https://github.com/murtazahassan/Learn-OpenCV-cpp-in-4-Hours/blob/main/Resources/test.png)地址直接下载到。

## 示例代码，对原图进行灰度化、高斯模糊、Canny边缘检测、膨胀和侵蚀处理
示例代码如下：
```cpp
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

///// Basic OpenCV Functions ////////
int main()
{
	std::string imgPath = "Resources/test.png";
	//std::string imgPath = "images/lena.jpg";

	cv::Mat imgSrc;
	cv::Mat imgGray, imgBlur, imgCanny, imgDilate, imgErode;
	imgSrc = cv::imread(imgPath);	// 读取原图像
	cvtColor(imgSrc, imgGray, COLOR_BGR2GRAY);	// 将原图转换成灰度图

	// 高斯模糊算法
	GaussianBlur(imgSrc, imgBlur, Size(3, 3), 3, 0);

	// Canny边缘检测算法
	Canny(imgBlur, imgCanny, 25, 75);

	// dilate and erode - 膨胀和腐蚀
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDilate, kernel);	// 膨胀图

	erode(imgDilate, imgErode, kernel);	// 腐蚀化


	cv::imshow("Image Source", imgSrc);			// 显示原图
	cv::imshow("Image Gray", imgGray);			// 显示灰度图
	cv::imshow("Image Gaussian Blur", imgBlur); // 显示高斯模糊处理后的图像
	cv::imshow("Image Canny Detect", imgCanny);	// 显示Canny边缘检测后的图像
	cv::imshow("Image Dilation", imgDilate);	// 膨胀
	cv::imshow("Image Erode", imgErode);		// 腐蚀

	cv::waitKey(0);

	return 0;
}
````
在`VS2017`中运行结果如下图所示：
![原图和灰度图](https://img-blog.csdnimg.cn/direct/20ace0b8b14c4d5e87bb26a3b3fd0e1e.png)
![高斯模糊和Canny边缘检测](https://img-blog.csdnimg.cn/direct/5337376c0a54441780d0f34fbe25df98.png)
![膨胀和腐蚀图](https://img-blog.csdnimg.cn/direct/189f9836b06d4a418cca175a4fc519b9.png)
## 参考资料
- [LEARN OPENCV C++ in 4 HOURS | Including 3x Projects | Computer Vision](https://www.youtube.com/watch?v=2FYm3GOonhk)
- [Learn-OpenCV-cpp-in-4-Hours](https://github.com/murtazahassan/Learn-OpenCV-cpp-in-4-Hours)
- [LEARN OPENCV in 3 HOURS with Python | Including 3xProjects | Computer Vision](https://youtu.be/WQeoO7MI0Bs)
- [Learn-OpenCV-in-3-hours](https://github.com/murtazahassan/Learn-OpenCV-in-3-hours)
