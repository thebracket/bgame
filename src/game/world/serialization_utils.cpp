#include "serialization_utils.h"

void save_color(const color_t &color, fstream &lbfile)
{
	char buffer[3];
	buffer[0] = static_cast<char>(std::get<0>(color));
	buffer[1] = static_cast<char>(std::get<1>(color));
	buffer[2] = static_cast<char>(std::get<2>(color));
	lbfile.write(buffer, 3);
}

void load_color(color_t& color, fstream& lbfile)
{
	char buffer[3];
	lbfile.read(buffer, 3);
	std::get<0>(color) = static_cast<unsigned char>(buffer[0]);
	std::get<1>(color) = static_cast<unsigned char>(buffer[1]);
	std::get<2>(color) = static_cast<unsigned char>(buffer[2]);
}
