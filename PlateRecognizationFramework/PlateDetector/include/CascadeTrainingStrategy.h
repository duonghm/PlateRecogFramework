#ifndef CASCADE_TRAINING_STRATEGY_H
#define CASCADE_TRAINING_STRATEGY_H

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/objdetect.hpp"
#include "IPlateDetectStrategy.h"
#include "PlateDetectorInputData.h"

namespace pr{
	class CascadeTrainingStrategy: IPlateDetectStrategy{
	private:
		std::string cascadeFile;
		cv::CascadeClassifier classfier;
		cv::Size minSize;
		cv::Size maxSize;
	public:
		CascadeTrainingStrategy();
		CascadeTrainingStrategy(std::string);
		void LoadCascadeFile(std::string);
		std::vector<PlateRegion> GetPlateRegions(PlateDetectorInputData* data) override;
	};
}

#endif 
