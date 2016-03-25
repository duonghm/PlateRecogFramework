#include "opencv2/opencv.hpp"
#include "CameraConvertor.h"

using namespace pr;

void Test_CameraModule_1(){
	std::string sourceURL = "C:/ImgTest/vn-1.jpg";
	CameraConvertor camconv = CameraConvertor(sourceURL, IMAGE, cv::IMREAD_ANYCOLOR);
	cv::Mat img = camconv.GetImage();
	imshow("test", img);
}

void Test_CameraModule_2(){
	std::string sourceURL = "F:/FIT8/fit8.ts";
	CameraConvertor camconv = CameraConvertor(sourceURL, VIDEO, cv::IMREAD_ANYCOLOR);
	while (true){
		cv::Mat img = camconv.GetImage();
		if (cv::waitKey(1) > 0) break;
		imshow("test", img);
	}
	
}

void Test_CameraModule()
{
	Test_CameraModule_2();
}
