#include "PlateExtractStrategy.h"
#include "PlateRegion.h"
#include "ITextRecognizer.h"
#include <iostream>

using namespace pr;

std::string PlateExtractStrategy::GetTextData(PlateRegion plate, ITextRecognizer* recognizer)
{	
	return recognizer->GetText(plate.imgData);
}

