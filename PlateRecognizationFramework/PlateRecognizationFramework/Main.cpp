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
	PlateRecognizator recognizator;
	recognizator.Init("../data/samples/vn-39.jpg", "../data/plateCascade/vn.xml");
	std::vector<std::string> result = recognizator.GetResult();
	for (int i = 0; i < result.size(); i++){
		std::cout << "Plate " << std::to_string(i) << ": " << result[i] << std::endl;
	}
#endif

	waitKey(0);
	return 0;
}