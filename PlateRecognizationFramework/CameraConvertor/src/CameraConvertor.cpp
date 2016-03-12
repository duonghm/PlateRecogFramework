#include "CameraConvertor.h"

using namespace pr;

CameraConvertor::CameraConvertor(std::string source, INPUT_TYPE type, cv::ImreadModes mode)
{
	LoadData(source, type, mode);
}

cv::Mat* CameraConvertor::GetImage(int frame)
{
	switch (type)
	{
	case pr::IMAGE:
		std::cout << "Do not support get multi frame for one image" << std::endl;
		return NULL;
		break;
	case pr::VIDEO:
		if (frame > 0){
			int size = frame > current_cache_size ? frame : current_cache_size;
			cv::Mat* arr = new cv::Mat[size];
			std::memcpy(arr, cacheImg, size);
		}
		break;
	case pr::CAMERA:
		break;
	default:
		break;
	}
}

cv::Mat CameraConvertor::GetImage()
{
	cv::Mat img;
	switch (type)
	{
	case IMAGE:
		img = cv::imread(source, mode);
		break;
	case VIDEO:
		cap.read(img);
		if (!img.empty()){
			AddImageToCache(img);
		}		
		break;
	case CAMERA:
		break;
	default:
		break;
	}
	return img;
}

void pr::CameraConvertor::LoadData(std::string source, INPUT_TYPE type, cv::ImreadModes mode)
{
	this->source = source;
	this->type = type;
	this->mode = mode;
	switch (type)
	{
	case IMAGE:		
		break;
	case VIDEO:
		cap = cv::VideoCapture(source);
		break;
	case CAMERA:
		break;
	default:
		break;
	}
}

void pr::CameraConvertor::AddImageToCache(cv::Mat img)
{
	if (current_cache_size >= MAX_CACHE_IMAGE){
		//std::memmove(cacheImg, cacheImg + 1, (MAX_CACHE_IMAGE-1) * sizeof(cv::Mat));
		//cacheImg[MAX_CACHE_IMAGE] = img;
	}
	else{
		//cacheImg[current_cache_size++] = img;
	}
}


