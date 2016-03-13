#include "TesseractTextRecognizer.h"
#include "opencv2/opencv.hpp"
#include "baseapi.h"
#include "leptonica/allheaders.h"
#include <iostream>

using namespace pr;

std::string TesseractTextRecognizer::GetText(cv::Mat img)
{
	tesseract::TessBaseAPI api;
	api.Init(NULL, "leu");
	//cv::cvtColor(img, img, cv::COLOR_RGB2GRAY, 0);
	api.SetImage((uchar*)img.data, img.cols, img.rows, 1, img.cols);
	char* outText = api.GetUTF8Text();		
	return std::string(outText);
}

