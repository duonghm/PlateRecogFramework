#ifndef ICARDETECT_H
#define ICARDETECT_H

#include "CarRegion.h"

namespace pr{
	class ICarDetect{
	public:
		virtual CarRegion* GetCarsRegions();
	};
}
#endif