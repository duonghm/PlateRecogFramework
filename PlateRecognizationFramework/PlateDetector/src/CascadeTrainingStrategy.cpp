#include "CascadeTrainingStrategy.h"
#include "CascadeTrainingInputData.h"
#include "PlateRegion.h"
#include "opencv2/opencv.hpp"

using namespace pr;

std::vector<PlateRegion> CascadeTrainingStrategy::GetPlateRegions(PlateDetectorInputData* data)
{
	std::vector<cv::Rect> plates;
	CascadeTrainingInputData* cdata = (CascadeTrainingInputData*)data;
	this->classfier.detectMultiScale(cdata->img, plates, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, minSize, maxSize);
	std::vector<PlateRegion> platesRegions;
	for (int i = 0; i < plates.size(); i++){
		PlateRegion p;
		p.region = plates.at(i);
		p.imgData = cdata->img(p.region).clone();
		platesRegions.push_back(p);
	}
	return platesRegions;
}

CascadeTrainingStrategy::CascadeTrainingStrategy()
{

}

pr::CascadeTrainingStrategy::CascadeTrainingStrategy(std::string file)
{
	LoadCascadeFile(file);
}

void pr::CascadeTrainingStrategy::LoadCascadeFile(std::string file)
{
	this->cascadeFile = file;
	classfier.load(cascadeFile);
}

