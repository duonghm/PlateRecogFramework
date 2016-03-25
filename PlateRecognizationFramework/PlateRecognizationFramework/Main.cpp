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
	CameraConvertor cam = CameraConvertor("F:/FIT8/fit8.ts", VIDEO, cv::IMREAD_ANYCOLOR);
	PlateRecognizator recognizator;
	recognizator.Init("../data/plateCascade/eu.xml");
	while (true){
		cv::Mat img = cam.GetImage();
		cv::Mat dis = img.clone();
		if (img.empty()){continue;}
		recognizator.SetImg(img);
		std::vector<pr::PlateRegion> plates = recognizator.GetPlateRegions();
		//for (int i = 0; i<plates.size(); i++){
		//	cv::rectangle(dis, plates[i].region, Scalar(255, 255, 0), 2, 8, 0);
		//}
		cv::resize(dis, dis, dis.size() / 2);
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