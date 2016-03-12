#ifndef IPLATE_EXTRACT_H
#define IPLATE_EXTRACT_H

#include <iostream>
#include "PlateRegion.h"

namespace pr{
	class IPlateExtract{
	public:
		virtual std::string GetTextData(PlateRegion);
		virtual std::vector<std::string> GetTextData(std::vector<PlateRegion>);
	};
}

#endif