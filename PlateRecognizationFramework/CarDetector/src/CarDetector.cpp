#include "CarDetector.h"
#include "CarDetectorInputData.h"
#include "CarRegion.h"

using namespace pr;

CarRegion* pr::CarDetector::GetCarsRegions()
{
	return detectStrategy->GetCarsRegions(inputData);
}

void pr::CarDetector::SetInputData(CarDetectorInputData* data)
{
	this->inputData = data;
}
