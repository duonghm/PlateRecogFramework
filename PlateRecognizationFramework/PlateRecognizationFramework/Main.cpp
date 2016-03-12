#include <iostream>
#include <CameraConvertor.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace pr;
using namespace std;

RNG rng(12345);

int main(){
	Mat img = CameraConvertor("C:/ImgTest/vn-1.jpg", IMAGE, cv::IMREAD_ANYCOLOR).GetImage();
	/*CameraConvertor cc = CameraConvertor("C:/174034-vv-1.avi", VIDEO, cv::IMREAD_ANYCOLOR);	

	Ptr<BackgroundSubtractor> pMOG2 = createBackgroundSubtractorMOG2();
	Mat fgMaskMOG2;
	std::vector<std::vector<cv::Point> > contours;

	Mat img;
		
	for (;;){
		std::cout << "load img" << std::endl;
		img = cc.GetImage();
		if (img.empty()) break;
		
		pMOG2->apply(img, fgMaskMOG2);

		cv::GaussianBlur(fgMaskMOG2, fgMaskMOG2, cv::Size(5,5), 0, 0);

		//kmeans(fgMaskMOG2, 1, )

		imshow("demo", fgMaskMOG2);


		if (waitKey(30) >= 0) break;
	}	*/
	Mat gray;
	cv::cvtColor(img, gray, CV_RGB2GRAY, 0);
	cv::blur(gray, gray, Size(5, 5));
	cv::adaptiveThreshold(gray, gray, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, 255, 1);

	Mat dst, cdst;	
	cv::Canny(gray, dst, 0, 15, 3);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(dst, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Find the convex hull object for each contour
	/*vector<vector<Point> >hull(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		convexHull(Mat(contours[i]), hull[i], false);
	}*/


	Mat drawing = Mat::zeros(dst.size(), CV_8UC3);
	double average_area = 0;
	for (int i = 0; i < contours.size(); i++){
		average_area += contourArea(contours[i]);
	}
	average_area /= contours.size();
	cout << average_area;
	for (int i = 0; i < contours.size(); i++)
	{		
		//if (contourArea(contours[i]) > average_area){
			RotatedRect box = minAreaRect(contours[i]);			
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			rectangle(img, box.boundingRect(), color, 2, 8, 0);
			//drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		//}		
	}

	imshow("gray", gray);
	imshow("canny", dst);
	imshow("contours", img);

	waitKey(0);
	return 0;
}