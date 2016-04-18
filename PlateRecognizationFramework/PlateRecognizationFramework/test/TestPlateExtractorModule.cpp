#include "opencv2/opencv.hpp"
#include "TestPlateExtractorModule.h"
#include "PlateExtractor.h"
#include "TesseractTextRecognizer.h"
#include "KNNTextRecognizer.h"
#include "DirectExtractStrategy.h"
#include "PlateRegion.h"
#include <iostream>

#include "leptonica/allheaders.h"

#define ENABLE_TESSERACT_READER
//#define USE_GET_COMPONENT
//#define USE_ITERATOR
#define CUSTOM_GROUPER

using namespace pr;

int scaleFactor = 3;

int minCharWidth = 2 * scaleFactor;
int minCharHeight = 10* scaleFactor;
int maxCharWidth = 40 * scaleFactor;
int maxCharHeight = 40 * scaleFactor;

std::vector<std::vector<cv::Point> > contours_poly;
std::vector<cv::Rect> boundRect;

bool isValidCharacterBox(cv::Rect rect){
	return rect.width > minCharWidth && rect.height > minCharHeight && rect.width < maxCharWidth && rect.height < maxCharHeight;
}

void Test_PlateExtractorModule_1();
void Test_PlateExtractorModule_2();
void Test_PlateExtractorModule_3();
void Test_PlateExtractorModule_4();

void Test_PlateExtractorModule()
{
	Test_PlateExtractorModule_4();
}

void showContourRect(cv::Mat img){
	std::cout << img.size() << std::endl;
	cv::Mat customeImgProcess = img.clone();
	cv::Mat disp = img.clone();	
	cv::cvtColor(disp, disp, cv::COLOR_GRAY2RGB, 0);
	//cv::cvtColor(disp, customeImgProcess, cv::COLOR_RGB2GRAY, 0);
	//cv::blur(disp, disp, cv::Size(5, 5));
	cv::equalizeHist(customeImgProcess, customeImgProcess);
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::adaptiveThreshold(customeImgProcess, customeImgProcess, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 37 * scaleFactor, 1);	

	int erosion_size = 1;
	cv::Mat element = cv::getStructuringElement(
		cv::MORPH_RECT,
		cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		cv::Point(erosion_size, erosion_size));
	cv::dilate(customeImgProcess, customeImgProcess, element);

	//cv::Canny(customeImgProcess, customeImgProcess, 30, 60, 3);

	cv::imshow("thres", customeImgProcess);
	cv::findContours(customeImgProcess, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));	

	contours_poly = std::vector<std::vector<cv::Point> >(contours.size());
	boundRect = std::vector<cv::Rect>(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		cv::approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 1, true);
		boundRect[i] = cv::boundingRect(cv::Mat(contours_poly[i]));		
	}

	for (int i = 0; i < contours.size(); i++)
	{
		if (isValidCharacterBox(boundRect[i])){
			cv::Scalar color = cv::Scalar(255, 0, 255);
			drawContours(disp, contours_poly, i, color, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
			rectangle(disp, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);			
		}
	}

	cv::imshow("custom", disp);
}

void Test_PlateExtractorModule_1(){

	std::string imgURL = "../cropData/virtualPlate/virtual_5_0.jpg";
	PlateRegion plate;
	plate.imgData = cv::imread(imgURL, 0);

	PlateExtractor extractor;
	DirectExtractStrategy* directStra = new DirectExtractStrategy();
	TesseractTextRecognizer* tessRecog = new TesseractTextRecognizer();
	tessRecog->InitData("../data/tessdata/", "lus");
	tessRecog->InitData(NULL, "leu");

	PlateExtractStrategy* strategy = (PlateExtractStrategy*)directStra;
	ITextRecognizer* recognizer = (ITextRecognizer*)tessRecog;

	extractor.SetStrategy(strategy);
	extractor.SetRecognizer(recognizer);
		
	std::string text = extractor.GetTextData(plate);
	std::cout << text << std::endl;
	imshow("Plate Extractor", plate.imgData);
}

void Test_PlateExtractorModule_2(){

	std::string imgURL = "../cropData/cropPlate/547_0.jpg";
	//std::string imgURL = "../data/plateSamples/demo.jpg";
	PlateRegion plate;

	cv::Mat img = cv::imread(imgURL, 0);
	//Pre processing
	//cv::resize(img, img, img.size() * 10);
	int margin_left = 0;
	int margin_right = 0;
	int margin_top = 0;
	int margin_bot = 0;
	cv::Rect rect = cv::Rect(margin_left, margin_top, img.size().width - margin_right - margin_left, img.size().height - margin_top - margin_bot);
	img = img(rect);
	//cv::normalize(img, img);
	cv::resize(img, img, img.size() * 2);	
	//cv::equalizeHist(img, img);	
	//cv::GaussianBlur(img, img, cv::Size(5, 5), 3, 3);
	//cv::addWeighted(img, 2, img, -0.5, 0, img);	
	//cv::dilate(img, img, cv::Mat());
	//cv::threshold(img, img, 150, 255, cv::THRESH_BINARY);
	//cv::adaptiveThreshold(img, img, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 7, 0);
	//cv::Canny(img, img, 10, 20, 3, true);

	//cv::cornerHarris(img, img, 2, 3, 0.04);
	cv::Mat edge;	
	Canny(img, edge, 30, 120, 3);
	imshow("Edge", edge);

	cv::Mat disp = img.clone();
	cv::cvtColor(disp, disp, cv::COLOR_GRAY2RGB);

	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(edge, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	for (unsigned int i = 0; i < contours.size(); i++){		
		drawContours(disp, contours, i, cv::Scalar(255, 255, 0), 2, 8);		
	}
	cv::imshow("Disp", disp);
		
	cv::imwrite("../cropData/cropPlate/process.jpg", img);

	plate.imgData = img;
	/*PlateExtractor extractor;
	DirectExtractStrategy* directStra = new DirectExtractStrategy();
	TesseractTextRecognizer* tessRecog = new TesseractTextRecognizer();
	tessRecog->InitData("../data/tessdata/", "leu");
	//tessRecog->InitData(NULL, "leu");

	PlateExtractStrategy* strategy = (PlateExtractStrategy*)directStra;
	ITextRecognizer* recognizer = (ITextRecognizer*)tessRecog;

	extractor.SetStrategy(strategy);
	extractor.SetRecognizer(recognizer);

	std::string text = extractor.GetTextData(plate);
	std::cout << text << std::endl;*/
	imshow("Plate Extractor", plate.imgData);
}

void Test_PlateExtractorModule_3(){
	
	std::string imgURL = "../cropData/cropPlate/2946_0.jpg";	
	PlateRegion plate;
	cv::Mat img = cv::imread(imgURL, 1);	
	cv::resize(img, img, img.size() * scaleFactor);
	cv::Mat processImg;
	cv::cvtColor(img, processImg, cv::COLOR_RGB2GRAY, 0);

#ifdef CUSTOM_GROUPER
	cv::Mat customImg = img.clone();
	cv::cvtColor(customImg, customImg, cv::COLOR_RGB2GRAY, 0);
	showContourRect(customImg);
	
#endif // CUSTOM_GROUPER

	// --- Pre Process --------------------------------------	
	int erosion_size = 2;
	cv::Mat element = cv::getStructuringElement(
		cv::MORPH_RECT, 
		cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1), 
		cv::Point(erosion_size, erosion_size));
	cv::dilate(processImg, processImg, element);

	cv::equalizeHist(processImg, processImg);
	cv::GaussianBlur(processImg, processImg, cv::Size(1, 1), 2.0, 2.0);
	cv::addWeighted(processImg, 2.5, processImg, -0.5, 0, processImg);
	//showContourRect(processImg);
	//cv::erode(processImg, processImg, element);
	//cv::inRange(processImg, cv::Scalar(30, 0, 0), cv::Scalar(200, 255, 255), processImg);
	//-------------------------------------------------------

	cv::imwrite("../cropData/cropPlate/process.jpg", img);

	//plate.imgData = processImg;
	plate.imgData = customImg;

#ifdef ENABLE_TESSERACT_READER
	tesseract::TessBaseAPI* api;
	api = new tesseract::TessBaseAPI();
	api->SetPageSegMode(tesseract::PSM_SINGLE_CHAR);	
	api->Init("../data/tessdata/", "leu");
	api->SetVariable("tessedit_char_whitelist", "0123456789");
	cv::imshow("Input Data", plate.imgData);
	api->SetImage((uchar*)plate.imgData.data, plate.imgData.cols, plate.imgData.rows, 1, plate.imgData.cols);
	
#ifdef USE_GET_COMPONENT
	Boxa* boxes = api->GetComponentImages(tesseract::RIL_SYMBOL, true, NULL, NULL);
	printf("Found %d textline image components.\n", boxes->n);
	for (int i = 0; i < boxes->n; i++) {
		BOX* box = boxaGetBox(boxes, i, L_CLONE);

		cv::Rect rect = cv::Rect(box->x, box->y, box->w, box->h);
		if (rect.width > 8 && rect.height > 8){

			api->SetRectangle(box->x, box->y, box->w, box->h);
			api->AllWordConfidences();
			char* ocrResult = api->GetUTF8Text();

			int conf = api->MeanTextConf();
			fprintf(stdout, "Box[%d]: x=%d, y=%d, w=%d, h=%d, confidence: %d, text: %s\n",
				i, box->x, box->y, box->w, box->h, conf, ocrResult);

			cv::rectangle(img, rect, cv::Scalar(255, 255, 0), 2, 8, 0);
		}
	}
#endif // USE_GET_COMPONENT

#ifdef USE_ITERATOR	
	api->Recognize(0);
	tesseract::ResultIterator* ri = api->GetIterator();
	tesseract::PageIteratorLevel level = tesseract::RIL_SYMBOL;
	if (ri != 0) {
		do {
			const char* word = ri->GetUTF8Text(level);
			float conf = ri->Confidence(level);
			int x1, y1, x2, y2;
			ri->BoundingBox(level, &x1, &y1, &x2, &y2);	
			if (x2 - x1 > minCharWidth && y2 - y1 > minCharHeight){
				cv::Rect rect = cv::Rect(x1, y1, x2 - x1, y2 - y1);
				cv::rectangle(img, rect, cv::Scalar(255, 255, 0), 2, 8, 0);
				printf("word: '%s';  \tconf: %.2f; BoundingBox: %d,%d,%d,%d;\n",
					word, conf, x1, y1, x2, y2);
			}			
			delete[] word;

		} while (ri->Next(level));
	}
#endif // USE_ITERATOR

#ifdef CUSTOM_GROUPER
	for (int i = 0; i < boundRect.size(); i++){
		if (isValidCharacterBox(boundRect[i])){	
			cv::Mat cropChar = plate.imgData(boundRect[i]);						
			equalizeHist(cropChar, cropChar);
			cv::adaptiveThreshold(cropChar, cropChar, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 37*scaleFactor, 1);
			/*cv::dilate(
				cropChar,
				cropChar,
				cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3))
				);*/
			cv::imwrite("../cropData/cropCharacter/char_" + std::to_string(i)+".jpg", cropChar);
			//imshow("char_" + std::to_string(i), cropChar);
			api->SetImage((uchar*)cropChar.data, cropChar.cols, cropChar.rows, 1, cropChar.cols);			
			char* ocrResult = api->GetUTF8Text();			
			std::string r(ocrResult);
			delete ocrResult;
			std::cout << i << ": " << r << "\n";
		}
	}
#endif
	
#endif // ENABLE_TESSERACT_READER
		
	imshow("Plate Extractor", img);
	imshow("Processed", processImg);
}

void Test_PlateExtractorModule_4(){	
	

	std::string imgURL = "../cropData/cropPlate/873_0.jpg";
	PlateRegion plate;
	plate.imgData = cv::imread(imgURL, 1);
	
	PlateExtractor extractor;
	DirectExtractStrategy* directStra = new DirectExtractStrategy();
	KNNTextRecognizer* knnRecog = new KNNTextRecognizer();
	knnRecog->Init("../data/knndata/classifications.xml", "../data/knndata/images.xml");
	
	PlateExtractStrategy* strategy = (PlateExtractStrategy*)directStra;
	ITextRecognizer* recognizer = (ITextRecognizer*)knnRecog;

	extractor.SetStrategy(strategy);
	extractor.SetRecognizer(recognizer);

	std::string text = extractor.GetTextData(plate);
	std::cout << text << std::endl;

	imshow("Plate Extractor", plate.imgData);
}
