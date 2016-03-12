#ifndef BACKGROUNDSUBTRACTIONSTRATEGY_H
#define BACKGROUNDSUBTRACTIONSTRATEGY_H

#include "ICarDetectStrategy.h"

namespace pr{
	class BackgroundSubtractionStrategy : ICarDetectStrategy{
	public:
		CarRegion* GetCarsRegions(CarDetectorInputData*) override;
	};
}

#endif