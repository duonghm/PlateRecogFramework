#ifndef CARREGION_H
#define CARREGION_H

#include <opencv2/opencv.hpp>

namespace pr{
	class CarRegion{
	public:
		cv::Rect data;
		cv::Mat imgData;
	};
}
#endif