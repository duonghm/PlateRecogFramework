#ifndef DIRECT_EXTRACT_STRATEGY_H
#define DIRECT_EXTRACT_STRATEGY_H

#include "PlateExtractStrategy.h"
#include "PlateRegion.h"
#include "ITextRecognizer.h"

namespace pr{
	class DirectExtractStrategy : PlateExtractStrategy{
	public:
		std::string GetTextData(PlateRegion, ITextRecognizer*) override;
	};
}

#endif