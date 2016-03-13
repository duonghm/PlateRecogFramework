#include "DirectExtractStrategy.h"
#include <iostream>

using namespace pr;

std::string pr::DirectExtractStrategy::GetTextData(PlateRegion plate, ITextRecognizer* recognizer)
{
	std::cout << "Call Direct Strategy" << std::endl;
	return recognizer->GetText(plate.imgData);
}

