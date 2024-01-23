# VC++中使用OpenCV进行人脸检测
![test.png](https://img-blog.csdnimg.cn/direct/8a22e0014f8b41e8b3df7fdde82cea22.png#pic_center)
对于上面的图像，如何使用OpenCV进行人脸检测呢？
使用OpenCV进行人脸检测十分简单，OpenCV官网给了一个Python人脸检测的示例程序，
![Python人脸检测目录](https://img-blog.csdnimg.cn/direct/408b557a7dc1421594da85f792c7527f.png)
`objectDetection.py`代码如下：
```python
from __future__ import print_function
import cv2 as cv
import argparse

def detectAndDisplay(frame):
    frame_gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    frame_gray = cv.equalizeHist(frame_gray)

    #-- Detect faces
    faces = face_cascade.detectMultiScale(frame_gray)
    for (x,y,w,h) in faces:
        center = (x + w//2, y + h//2)
        frame = cv.ellipse(frame, center, (w//2, h//2), 0, 0, 360, (255, 0, 255), 4)

        faceROI = frame_gray[y:y+h,x:x+w]
        #-- In each face, detect eyes
        eyes = eyes_cascade.detectMultiScale(faceROI)
        for (x2,y2,w2,h2) in eyes:
            eye_center = (x + x2 + w2//2, y + y2 + h2//2)
            radius = int(round((w2 + h2)*0.25))
            frame = cv.circle(frame, eye_center, radius, (255, 0, 0 ), 4)

    cv.imshow('Capture - Face detection', frame)

parser = argparse.ArgumentParser(description='Code for Cascade Classifier tutorial.')
parser.add_argument('--face_cascade', help='Path to face cascade.', default='data/haarcascades/haarcascade_frontalface_alt.xml')
parser.add_argument('--eyes_cascade', help='Path to eyes cascade.', default='data/haarcascades/haarcascade_eye_tree_eyeglasses.xml')
parser.add_argument('--camera', help='Camera divide number.', type=int, default=0)
args = parser.parse_args()

face_cascade_name = args.face_cascade
eyes_cascade_name = args.eyes_cascade

face_cascade = cv.CascadeClassifier()
eyes_cascade = cv.CascadeClassifier()

#-- 1. Load the cascades
if not face_cascade.load(cv.samples.findFile(face_cascade_name)):
    print('--(!)Error loading face cascade')
    exit(0)
if not eyes_cascade.load(cv.samples.findFile(eyes_cascade_name)):
    print('--(!)Error loading eyes cascade')
    exit(0)

camera_device = args.camera
#-- 2. Read the video stream
cap = cv.VideoCapture(camera_device)
if not cap.isOpened:
    print('--(!)Error opening video capture')
    exit(0)

while True:
    ret, frame = cap.read()
    if frame is None:
        print('--(!) No captured frame -- Break!')
        break

    detectAndDisplay(frame)

    if cv.waitKey(10) == 27:
        break

```
所在目录为D:\env_build\opencv4.9.0\opencv\sources\samples\python\tutorial_code\objectDetection\cascade_classifier\objectDetection.py

## 人脸识别的背景
人脸识别可以用在身份认证，门禁等场合中，可以通过训练大量的人脸数据获取人脸的特征。但是实际场景可以比较复杂，由于灯光、视角、视距、摄像头抖动以及数字噪声的变化，图像细节变得不稳定；还有戴了口罩、帽子之后对于人脸的检测就变得更麻烦了。Haar 特征是一种用于实现实时人脸跟踪的特征。每一个 Haar 特征都描述了相邻图像区域的对比模式。例如，边，顶点和细线都能生成具有判别性的特征。
## haar级联数据获取
在 sources 的一个文件夹 data/haarcascades。该文件夹包含了所有 OpenCV 的人脸检测的 XML 文件，这些可用于检测静止图像、视频和摄像头所得到图像中的人脸。如下图所示：
![haar级联数据](https://img-blog.csdnimg.cn/direct/c264db262f724d52af91e7804c07de51.png)
- 人脸检测器（默认）：haarcascade_frontalface_default.xml 
- 人脸检测器（快速 Harr）：haarcascade_frontalface_alt2.xml
- 人脸检测器（侧视）：haarcascade_profileface.xml 
- 眼部检测器（左眼）：haarcascade_lefteye_2splits.xml 
- 眼部检测器（右眼）：haarcascade_righteye_2splits.xml 
- 身体检测器：haarcascade_fullbody.xml 
- 上半身检测器：haarcascade_upperbody.xml
其中，本文中我们使用默认的人脸检测器xml配置文件`haarcascade_frontalface_default.xml `，可以从[https://github.com/murtazahassan/Learn-OpenCV-cpp-in-4-Hours/blob/main/Resources/haarcascade_frontalface_default.xml](https://github.com/murtazahassan/Learn-OpenCV-cpp-in-4-Hours/blob/main/Resources/haarcascade_frontalface_default.xml)处下载

## 资源图片地址
人脸资源图片地址为：[https://github.com/murtazahassan/Learn-OpenCV-cpp-in-4-Hours/blob/main/Resources/test.png](https://github.com/murtazahassan/Learn-OpenCV-cpp-in-4-Hours/blob/main/Resources/test.png)

## C++人脸检测示例代码
C++示例代码如下：
```cpp
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;


///////////////  Images 人脸检测  //////////////////////

int main()
{

	string path = "Resources/test.png";
	//string path = "Resources/multiFaces.jpg";
	Mat img = imread(path);

	CascadeClassifier faceCascade;	// 级联分类器
	faceCascade.load("Resources/haarcascade_frontalface_default.xml"); // 加载训练好的haar人脸正脸xml配置文件
	if (faceCascade.empty()) {
		cout << "XML file not loaded" << endl;
	}

	vector<Rect> faces;	// 人脸的矩形数组
	faceCascade.detectMultiScale(img, faces, 1.1, 10);	// 检测输入图像中不同大小的对象，检测到的对象返回矩形列表形式faces

	// 针对每个检测到的人脸矩形，在对应位置上绘制矩形区域
	for (int i = 0; i < faces.size(); i++)
	{
		rectangle(img, faces[i].tl(), faces[i].br(), Scalar(255, 0, 255), 3);	// 在原图上绘制人脸矩形区域，颜色为粉色，线条厚度为3像素
	}

	imshow("Image", img);
	waitKey(0);

	return 0;
}
```
运行结果如下：
![单张人脸检测运行结果](https://img-blog.csdnimg.cn/direct/12fa517b270e48ec99f205f8409ade3b.png)


## 参考资料
- [人脸识别-Haar级联](https://blog.csdn.net/am_student/article/details/122371807)
- [人脸识别-多张人脸检测](https://blog.csdn.net/am_student/article/details/122431294)
- [LEARN OPENCV C++ in 4 HOURS | Including 3x Projects | Computer Vision](https://www.youtube.com/watch?v=2FYm3GOonhk)
- [murtazahassan/Learn-OpenCV-cpp-in-4-Hours](https://github.com/murtazahassan/Learn-OpenCV-cpp-in-4-Hours)
-  [LEARN OPENCV in 3 HOURS with Python | Including 3xProjects | Computer Vision](https://youtu.be/WQeoO7MI0Bs)
- [Learn-OpenCV-in-3-hours](https://github.com/murtazahassan/Learn-OpenCV-in-3-hours)
- [OpenCV官网](https://opencv.org/)
- [OpenCV-Get Started](https://opencv.org/get-started/)
- [OpenCV Github仓库源代码](https://github.com/opencv/opencv)
- [OpenCV tutorial](https://docs.opencv.org/master/d9/df8/tutorial_root.html)
