
#include "BackgroundSubtractionStrategy.h"
#include "BackgroundSubtractionData.h"
#include <opencv2/opencv.hpp>

using namespace pr;

CarRegion* BackgroundSubtractionStrategy::GetCarsRegions(CarDetectorInputData* rawData)
{
	BackgroundSubtractionData* bsData = (BackgroundSubtractionData*) rawData;
	cv::Ptr<cv::BackgroundSubtractor> pMOG2 = cv::createBackgroundSubtractorMOG2();
	pMOG2->apply(bsData->data, bsData->data);
	return NULL;
}

