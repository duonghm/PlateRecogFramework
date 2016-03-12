#ifndef ITEXT_RECOGNIZER_H
#define ITEXT_RECOGNIZER_H

#include "opencv2/opencv.hpp"
#include <iostream>

namespace pr{
	class ITextRecognizer{
	public:
		virtual std::string GetText(cv::Mat img);
	};
}

#endif