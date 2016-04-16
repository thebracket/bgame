#pragma once

#include <string>
#include <png.h>

using namespace std;

class png_writer
{
public:
	png_writer(string nFilename, int w, int h);
	void setPixel(int x, int y, png_byte r, png_byte g, png_byte b, png_byte a);
	void save();
	~png_writer();
private:
	string filename;
	int height;
	int width;
	FILE * fp;
	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep * buf;
};
