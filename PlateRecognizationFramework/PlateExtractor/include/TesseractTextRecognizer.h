#ifndef TESSERACT_TEXT_RECOGNIZER_H
#define TESSERACT_TEXT_RECOGNIZER_H

#include "baseapi.h"
#include "ITextRecognizer.h"
#include <iostream>

namespace pr{
	class TesseractTextRecognizer : ITextRecognizer{
	private:
		char* dataPath = NULL;
		char* language = "leu";
		tesseract::TessBaseAPI* api;
	public:
		void InitData(char* path, char* lang);
		std::string GetText(cv::Mat img) override;
	};
}

#endif