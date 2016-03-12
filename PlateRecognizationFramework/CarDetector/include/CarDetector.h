#ifndef CARDETECTOR_H
#define CARDETECTOR_H

#include "ICarDetect.h"
#include "CarRegion.h"
#include "ICarDetectStrategy.h"

namespace pr{
	class CarDetector : ICarDetect{
	private:
		ICarDetectStrategy* detectStrategy;
		CarDetectorInputData* inputData;
	public:
		CarRegion* GetCarsRegions() override;
		void SetInputData(CarDetectorInputData* data);
	};
}

#endif