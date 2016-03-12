#ifndef BACKGROUNDSUBTRACTION_DATA
#define BACKGROUNDSUBTRACTION_DATA

#include "CarDetectorInputData.h"
#include <opencv2/opencv.hpp>

namespace pr{
	class BackgroundSubtractionData : CarDetectorInputData{
	public:
		cv::Mat data;
	};
}

#endif