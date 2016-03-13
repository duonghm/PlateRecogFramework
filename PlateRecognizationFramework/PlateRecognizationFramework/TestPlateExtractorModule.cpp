#include "opencv2/opencv.hpp"
#include "TestPlateExtractorModule.h"
#include "PlateExtractor.h"
#include "TesseractTextRecognizer.h"
#include "DirectExtractStrategy.h"
#include "PlateRegion.h"
#include <iostream>

using namespace pr;

void Test_PlateExtractorModule_1();

void Test_PlateExtractorModule()
{
	Test_PlateExtractorModule_1();	
}

void Test_PlateExtractorModule_1(){

	std::string imgURL = "../data/plateSamples/demo.jpg";
	PlateRegion plate;
	plate.imgData = cv::imread(imgURL, 0);

	PlateExtractor extractor;
	DirectExtractStrategy* directStra = new DirectExtractStrategy();
	TesseractTextRecognizer* tessRecog = new TesseractTextRecognizer();

	PlateExtractStrategy* strategy = (PlateExtractStrategy*)directStra;
	ITextRecognizer* recognizer = (ITextRecognizer*)tessRecog;

	extractor.SetStrategy(strategy);
	extractor.SetRecognizer(recognizer);
		
	std::string text = extractor.GetTextData(plate);
	std::cout << text << std::endl;
	imshow("Plate Extractor", plate.imgData);
}


