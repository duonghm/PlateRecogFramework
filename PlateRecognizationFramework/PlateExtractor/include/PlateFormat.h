#ifndef PLATE_FORMAT_H
#define PLATE_FORMAT_H

#include <iostream>

namespace pr{
	class PlateFormat{
	public:
		std::string formatString;
		int height;
		int height_padding;
		int margin;
		int width;
		int width_padding;
		int space_between_char;
		int space_between_line;
		int number_of_line;
	};
}

#endif