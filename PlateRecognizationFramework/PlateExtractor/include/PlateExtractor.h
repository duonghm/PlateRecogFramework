#ifndef PLATE_EXTRACTOR_H
#define PLATE_EXTRACTOR_H

#include "IPlateExtract.h"
#include "PlateExtractStrategy.h"
#include "ITextRecognizer.h"

namespace pr{
	class PlateExtractor : IPlateExtract{
	private:
		PlateExtractStrategy* strategy;
		ITextRecognizer* recognizer;
	public:
		void SetStrategy(PlateExtractStrategy* strategy);
		void SetRecognizer(ITextRecognizer* recognizer);
		std::string GetTextData(PlateRegion) override;
		std::vector<std::string> GetTextData(std::vector<PlateRegion>) override;
	};
}

#endif