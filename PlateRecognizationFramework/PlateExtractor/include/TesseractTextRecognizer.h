#ifndef TESSERACT_TEXT_RECOGNIZER_H
#define TESSERACT_TEXT_RECOGNIZER_H

#include "ITextRecognizer.h"
#include <iostream>
#include "baseapi.h"

namespace pr{
	class TesseractTextRecognizer : ITextRecognizer{
	private:
		char* dataPath;
		char* language;
		tesseract::TessBaseAPI* api;
	public:
		std::string GetText(cv::Mat img) override;
		void InitData(char*, char*);
	};
}

#endif