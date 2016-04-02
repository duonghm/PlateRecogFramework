#ifndef CAMERA_CONVERTOR_H
#define CAMERA_CONVERTOR_H

#include <opencv2/opencv.hpp>

namespace pr{

	enum INPUT_TYPE {
		IMAGE,
		VIDEO,
		CAMERA,
		VLC
	};

	class CameraConvertor{
	private:
		static const int MAX_CACHE_IMAGE = 10;
		INPUT_TYPE type;
		std::string source;
		cv::ImreadModes mode = cv::IMREAD_ANYCOLOR;
		cv::Mat cacheImg[MAX_CACHE_IMAGE];
		int current_cache_size = 0;
		cv::VideoCapture cap;
		void AddImageToCache(cv::Mat img);
	public:		
		CameraConvertor(std::string source, INPUT_TYPE type, cv::ImreadModes mode);
		CameraConvertor(std::string source, INPUT_TYPE type);
		void LoadData(std::string source, INPUT_TYPE type, cv::ImreadModes mode);
		cv::Mat GetImage();
		cv::Mat* GetImage(int frame);
	};

}

#endif



