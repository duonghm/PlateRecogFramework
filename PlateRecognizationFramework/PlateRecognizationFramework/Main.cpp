#include <iostream>
#include <CameraConvertor.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

//#define TEST_CAMERA_CONVERTOR
//#define TEST_PLATE_DETECTOR
//#define TEST_PLATE_EXTRACTOR
#define TEST_PLATE_RECOGNIZATOR

#ifdef TEST_CAMERA_CONVERTOR
#include "TestCameraConvertorModule.h"
#endif 

#ifdef TEST_PLATE_DETECTOR
#include "TestPlateDetectorModule.h"
#endif

#ifdef TEST_PLATE_EXTRACTOR
#include "TestPlateExtractorModule.h"
#endif

#ifdef TEST_PLATE_RECOGNIZATOR
#include "PlateRecognizator.h"
#endif

using namespace cv;
using namespace pr;
using namespace std;

int main(){
	
#ifdef TEST_CAMERA_CONVERTOR
	Test_CameraModule();
#endif 

#ifdef TEST_PLATE_DETECTOR
	Test_PlateDetectorModule();
#endif

#ifdef TEST_PLATE_EXTRACTOR
	Test_PlateExtractorModule();
#endif

#ifdef TEST_PLATE_RECOGNIZATOR
	//CameraConvertor cam = CameraConvertor("F:/FIT8/fit8.ts", VIDEO, cv::IMREAD_ANYCOLOR);
	CameraConvertor cam = CameraConvertor("F:/FIT8/fit8.ts", VIDEO, cv::IMREAD_GRAYSCALE);
	PlateRecognizator recognizator_wide;
	recognizator_wide.Init("../data/plateCascade/eu.xml");
	PlateRecognizator recognizator_rect;
	recognizator_rect.Init("../data/plateCascade/vn.xml");

	std::vector<pr::PlateRegion> plates_wide;
	std::vector<pr::PlateRegion> plates_rect;
	while (true){
		cv::Mat img = cam.GetImage();
		cv::resize(img, img, img.size() / 2);
		cv::Mat dis = img.clone();
		if (img.empty()){continue;}
		recognizator_wide.SetImg(img);
		recognizator_rect.SetImg(img);
		plates_wide = recognizator_wide.GetPlateRegions();
		plates_rect = recognizator_rect.GetPlateRegions();
		if (plates_wide.size() > 0) std::cout << "Wide: " << plates_wide.size() << std::endl;
		if (plates_rect.size() > 0) std::cout << "Rect: " << plates_rect.size() << std::endl;
		std::cout << "---------" << std::endl;
		for (int i = 0; i<plates_wide.size(); i++){
			cv::rectangle(dis, plates_wide[i].region, Scalar(255, 255, 0), 2, 8, 0);
		}
		for (int i = 0; i<plates_rect.size(); i++){
			cv::rectangle(dis, plates_rect[i].region, Scalar(255, 0, 255), 2, 8, 0);
		}
		//cv::resize(dis, dis, dis.size() / 2);
		cv::imshow("FIT8", dis);
		if (waitKey(1) > 0)break;
	}
	/*std::vector<std::string> result = recognizator.GetResult();
	for (int i = 0; i < result.size(); i++){
		std::cout << "Plate " << std::to_string(i) << ": " << result[i] << std::endl;
	}*/
#endif

	waitKey(0);
	return 0;
}