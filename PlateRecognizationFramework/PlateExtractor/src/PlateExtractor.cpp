#include "PlateExtractor.h"
#include "PlateRegion.h"
#include <iostream>
#include <vector>
#include "PlateExtractStrategy.h"

using namespace pr;


std::string PlateExtractor::GetTextData(PlateRegion plate)
{	
	return strategy->GetTextData(plate, recognizer);
}

std::vector<std::string> PlateExtractor::GetTextData(std::vector<PlateRegion> plates)
{
	std::vector<std::string> result;
	for (int i = 0; i < plates.size(); i++){
		std::string text = GetTextData(plates[i]);
		result.push_back(text);
	}
	return result;
}

void pr::PlateExtractor::SetStrategy(PlateExtractStrategy* stra)
{
	this->strategy = stra;
}

void pr::PlateExtractor::SetRecognizer(ITextRecognizer* recog)
{
	this->recognizer = recog;
}

