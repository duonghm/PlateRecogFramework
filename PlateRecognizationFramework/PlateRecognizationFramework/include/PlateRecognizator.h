#ifndef PLATE_RECOGNIZATOR_H
#define PLATE_RECOGNIZATOR_H

#include "CameraConvertor.h"
#include "CarDetector.h"
#include "PlateDetector.h"
#include "PlateExtractor.h"
#include <iostream>
#include <vector>

namespace pr{
	class PlateRecognizator{
	private:
		cv::Mat img;
		std::string cascadeFileURL;		

		CarDetector* carDetector;
		PlateDetector* plateDetector;
		PlateExtractor* plateExtractor;
				
	public:		
		void Init(std::string cascadeFileURL, cv::Size minSize, cv::Size maxSize, double scale, int neighbor);
		std::vector<std::string> GetResult();
		std::vector<std::string> GetResult(std::vector<PlateRegion>);
		std::vector <pr::PlateRegion> GetPlateRegions();
		std::string GetResult(cv::Mat& img);
		void SetImg(cv::Mat& img);
		void InitInput();
		void InitPlateDetector(cv::Size minSize, cv::Size maxSize, double scale, int neighbor);
		void InitPlateExtractor();
	};
}

#endif