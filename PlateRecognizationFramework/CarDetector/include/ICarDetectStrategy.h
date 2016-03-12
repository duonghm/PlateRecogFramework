#ifndef ICARDETECTSTRATEGY_H
#define ICARDETECTSTRATEGY_H

#include "CarRegion.h"
#include "CarDetectorInputData.h"

namespace pr{
	class ICarDetectStrategy{
	public:
		virtual CarRegion* GetCarsRegions(CarDetectorInputData*);
	};
}

#endif