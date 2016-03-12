#ifndef TESSERACT_TEXT_RECOGNIZER_H
#define TESSERACT_TEXT_RECOGNIZER_H

#include "ITextRecognizer.h"
#include <iostream>

namespace pr{
	class TesseractTextRecognizer : ITextRecognizer{
	public:
		std::string GetText(cv::Mat img) override;
	};
}

#endif