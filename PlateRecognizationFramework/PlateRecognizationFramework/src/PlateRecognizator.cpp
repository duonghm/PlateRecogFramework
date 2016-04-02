#include "PlateRecognizator.h"
#include "CascadeTrainingStrategy.h"
#include "DirectExtractStrategy.h"
#include "TesseractTextRecognizer.h"
#include "opencv2/opencv.hpp"
#include "CascadeTrainingInputData.h"

using namespace pr;

//#define DEBUG

std::vector<std::string> PlateRecognizator::GetResult()
{	
	std::vector<PlateRegion> plates = GetPlateRegions();
	
#ifdef DEBUG	
	for (int i = 0; i < plates.size(); i++){
		imshow("plate"+std::to_string(i), plates[i].imgData);
	}
#endif // DEBUG
	
	// Extract Plate data
	std::vector<std::string> platesText = plateExtractor->GetTextData(plates);
	return platesText;	
}

std::vector<pr::PlateRegion> PlateRecognizator::GetPlateRegions()
{
	//cv::Mat img = cam->GetImage();
	//cv::cvtColor(img, img, cv::COLOR_RGB2GRAY);

	// Get Plates Regions
	CascadeTrainingInputData* casData = new CascadeTrainingInputData();
	casData->img = img;
	PlateDetectorInputData* plateDetectorData = (PlateDetectorInputData*)casData;
	plateDetector->SetInputData(plateDetectorData);
	std::vector<PlateRegion> plates = plateDetector->GetPlateRegions();

	return plates;
}

void pr::PlateRecognizator::Init(std::string cascadeFileURL)
{	
	this->cascadeFileURL = cascadeFileURL;
	InitInput();
	InitPlateDetector();
	InitPlateExtractor();
}

void pr::PlateRecognizator::InitInput()
{	
	
}

void pr::PlateRecognizator::InitPlateDetector()
{
	plateDetector = new PlateDetector();
	CascadeTrainingStrategy* casStrategy = new CascadeTrainingStrategy(cascadeFileURL);
	casStrategy->SetMinSize(cv::Size(30, 30));
	casStrategy->SetMaxSize(cv::Size(70, 70));
	IPlateDetectStrategy* strategy = (IPlateDetectStrategy*)casStrategy;
	plateDetector->SetDetectStrategy(strategy);	
}

void pr::PlateRecognizator::InitPlateExtractor()
{
	plateExtractor = new PlateExtractor();

	DirectExtractStrategy* directStra = new DirectExtractStrategy();
	TesseractTextRecognizer* tessRecog = new TesseractTextRecognizer();
	tessRecog->InitData(NULL, "vie");

	PlateExtractStrategy* strategy = (PlateExtractStrategy*)directStra;
	ITextRecognizer* recognizer = (ITextRecognizer*)tessRecog;

	plateExtractor->SetStrategy(strategy);
	plateExtractor->SetRecognizer(recognizer);
}

void pr::PlateRecognizator::SetImg(cv::Mat img)
{
	this->img = img;
}
