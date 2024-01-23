
# VC++中使用OpenCV读取图像、读取本地视频、读取摄像头并实时显示
   最近闲着跟着油管博主[murtazahassan](https://www.youtube.com/@murtazahassan01)，学习了一下[LEARN OPENCV C++ in 4 HOURS | Including 3x Projects | Computer Vision](https://www.youtube.com/watch?v=2FYm3GOonhk)，对应的Github源代码地址为：[Learn-OpenCV-cpp-in-4-Hours](https://github.com/murtazahassan/Learn-OpenCV-cpp-in-4-Hours)

## 关于OpenCV

[OpenCV](https://opencv.org/)是一个开源的计算机视觉库，其官网地址为：[https://opencv.org/](https://opencv.org/)，对应Github源码地址为：[https://github.com/opencv/opencv](https://github.com/opencv/opencv)，目前来说OpenCV对C++、Python的支持比较友好，同时还支持Java、Javascript、C#等语言。
![OpenCV官网地址](https://img-blog.csdnimg.cn/direct/df4f080ffe424e8484f2e96225e25914.png)
![Github源代码地址](https://img-blog.csdnimg.cn/direct/036d66c89088489ab86a4b6e7b22bef2.png)

OpenCV官网文档地址：[https://docs.opencv.org/4.x/d9/df8/tutorial_root.html](https://docs.opencv.org/4.x/d9/df8/tutorial_root.html)
![OpenCV API文档](https://img-blog.csdnimg.cn/direct/0549f0cf8a494f28b2121e94e0e922cd.png)


OpenCV官方给的示例C++程序：
```cpp
// Change path/to/image to a real path of an image, then build this demo with OpenCV package and run it
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
int main()
{
    std::string image_path = "path/to/image";
    Mat img = imread(image_path, IMREAD_COLOR);

    imshow("Display window", img);
    int k = waitKey(0); // Wait for a keystroke in the window
    return 0;
}
```
关于Windows下Visual Studio 2022中配置OpenCV库的使用我就不赘余了，方便的话可以直接看[Setup OpenCV in Visual Studio 2022 for C/C++ Development](https://www.youtube.com/watch?v=unSce_GPwto)或[OpenCV C++ and Microsoft Visual Studio: A Complete Tutorial on Installation and Usage for Beginners](https://www.youtube.com/watch?v=trXs2r6xSnI)这个视频教程。当然也可以参考油管博主[murtazahassan](https://www.youtube.com/@murtazahassan01)对应的Github仓库[Learn-OpenCV-cpp-in-4-Hours](https://github.com/murtazahassan/Learn-OpenCV-cpp-in-4-Hours)里面的[Installing OpenCV on Windows for C++](https://github.com/murtazahassan/Learn-OpenCV-cpp-in-4-Hours/blob/main/Opecv%20Windows%20Installation.md)，如下图所示：

![Installing OpenCV on Windows for C++](https://img-blog.csdnimg.cn/direct/fb6bd6f9e350403c9ea2178c20b4871f.png)
关于在Mac下使用XCode运行OpenCV的可以参考[Installing OpenCV on Mac for C++](https://github.com/murtazahassan/Learn-OpenCV-cpp-in-4-Hours/blob/main/Opecv%20Mac%20Installation.md)
![Installing OpenCV on Mac for C++](https://img-blog.csdnimg.cn/direct/fa48b6fd4f4f4ffba44df11a575037f1.png)

## Visual Studio中使用OpenCV读取图像
首先使用VS2017新建一个控制台项目`OpencvDemo01`，并在项目根目录放置一些资源文件，资源文件下载地址为：[https://github.com/murtazahassan/Learn-OpenCV-cpp-in-4-Hours/tree/main/Resources](https://github.com/murtazahassan/Learn-OpenCV-cpp-in-4-Hours/tree/main/Resources)
![Resources资源文件下载地址](https://img-blog.csdnimg.cn/direct/6618d1153fce4f198fc0a76b0f9e37c4.png)



如下图所示：
![OpenCV项目目录](https://img-blog.csdnimg.cn/direct/eee7d80dc09849b1b7c756b2ba4fadbb.png)
![资源文件](https://img-blog.csdnimg.cn/direct/abc4cfdb36c1401b9eca5491a09638f1.png)
在Visual Studio中使用C++ OpenCV库读取图像并显示很简单，示例代码如下：
```cpp
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	std::string imgPath = "Resources/test.png";

	cv::Mat img;
	img = cv::imread(imgPath);
	cv::imshow("Show Image", img);

	cv::waitKey(0);

	cv::destroyAllWindows();
	
	return 0;
}
```
运行结果如下图所示：
![使用OpenCV读取图像](https://img-blog.csdnimg.cn/direct/a3cec31ad08d4802ab71c5a0643918ce.png)
## 使用OpenCV读取本地视频
示例代码如下：
```cpp
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
	std::string videoPath = "Resources/test_video.mp4";
	cv::VideoCapture videoCap(videoPath);

	cv::Mat frame;

	if (videoCap.isOpened()) {
		while (videoCap.read(frame)) {
			cv::imshow("Video Frame", frame);

			char chKey = cv::waitKey(10);
			if (chKey == 27) {
				break;
			}
		}
	}

	videoCap.release();

	cv::destroyAllWindows();


	return 0;
}
```
运行结果如下图所示：
![OpenCV读取本地视频](https://img-blog.csdnimg.cn/direct/6b0c53b0f8ee4c26b2ab84a57a92b710.png)

## OpenCV读取摄像头数据
OpenCV读取本地摄像头数据也很简单，示例代码如下：
```cpp
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

// 摄像头中读取每一帧图像，然后输出到屏幕，按ESC键退出

int main()
{
	cv::VideoCapture cap(0);

	cv::Mat frame;

	if (cap.isOpened()) {	// 如果VideoCapture初始化成功
		while (cap.read(frame)) {
			cv::imshow("WebCam Image", frame);

			char chKey = cv::waitKey(10);
			if (chKey == 27) {	// 27 对应ESC键的ASCII码
				// 如果按下了ESC键，则退出循环
				break;
			}
		}
	}

	cap.release();
	cv::destroyAllWindows();

	return 0;
}
```
运行结果如下图所示：
![OpenCV读取本地摄像头数据](https://img-blog.csdnimg.cn/direct/55874c9d3007459b9ac790741d340f50.png)


## 参考资料
- [LEARN OPENCV C++ in 4 HOURS | Including 3x Projects | Computer Vision](https://www.youtube.com/watch?v=2FYm3GOonhk)
- [murtazahassan/Learn-OpenCV-cpp-in-4-Hours](https://github.com/murtazahassan/Learn-OpenCV-cpp-in-4-Hours)
- [OpenCV官网](https://opencv.org/)
- [OpenCV-Get Started](https://opencv.org/get-started/)
- [OpenCV Github仓库源代码](https://github.com/opencv/opencv)
- [OpenCV tutorial](https://docs.opencv.org/master/d9/df8/tutorial_root.html)
