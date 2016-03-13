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
		std::string sourceURL;
		std::string cascadeFileURL;		

		CameraConvertor* cam;
		CarDetector* carDetector;
		PlateDetector* plateDetector;
		PlateExtractor* plateExtractor;
				
	public:
		void Init(std::string sourceURL, std::string cascadeFileURL);
		std::vector<std::string> GetResult();
		void InitInput();
		void InitPlateDetector();
		void InitPlateExtractor();
	};
}

#endif