#include "DirectExtractStrategy.h"
#include <iostream>

using namespace pr;

std::string pr::DirectExtractStrategy::GetTextData(PlateRegion plate, ITextRecognizer* recognizer)
{	
	return recognizer->GetText(plate.imgData);
}

