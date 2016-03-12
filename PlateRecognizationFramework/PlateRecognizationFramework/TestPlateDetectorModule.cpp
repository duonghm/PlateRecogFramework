#include "opencv2/opencv.hpp"
#include "PlateDetector.h"
#include "CascadeTrainingStrategy.h"
#include "CascadeTrainingInputData.h"

#include "PlateDetectorInputData.h"
#include "IPlateDetectStrategy.h"

using namespace pr;

void Test_PlateDetectorModule_1();

void Test_PlateDetectorModule()
{
	Test_PlateDetectorModule_1();
}

void Test_PlateDetectorModule_1(){
	std::string testImgURL = "../data/samples/vn-80.jpg";
	std::string cascadeFileURL = "../data/plateCascade/vn.xml";
	cv::Mat img = cv::imread(testImgURL, 1);

	PlateDetector detector;
	CascadeTrainingStrategy* casStrategy = new CascadeTrainingStrategy(cascadeFileURL);
	CascadeTrainingInputData* casData = new CascadeTrainingInputData();
	casData->img = img;

	IPlateDetectStrategy* strategy = (IPlateDetectStrategy*)casStrategy;
	PlateDetectorInputData* data = (PlateDetectorInputData*)casData;
	
	detector.SetDetectStrategy(strategy);
	detector.SetInputData(data);

	std::vector<PlateRegion> plates = detector.GetPlateRegions();
	for (int i = 0; i < plates.size(); i++){
		cv::rectangle(img, plates.at(i).region, cv::Scalar(255, 255, 0), 1, 8, 0);
	}
	
	imshow("Plate Detect", img);
}
