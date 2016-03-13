#ifndef PLATE_EXTRACT_STRATEGY_H
#define PLATE_EXTRACT_STRATEGY_H

#include <iostream>
#include "ITextRecognizer.h"
#include "PlateRegion.h"

namespace pr{
	class PlateExtractStrategy{
	public:
		virtual std::string GetTextData(PlateRegion, ITextRecognizer*);
	};
}

#endif