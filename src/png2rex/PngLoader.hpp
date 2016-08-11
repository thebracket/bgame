#pragma once

#include <string>
#include <png.h>
#include <tuple>
 
class PngLoader
{
public:
    PngLoader();
    ~PngLoader();
 
    bool load( std::string file );
    const char *getData() const { return data; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getDepth() const { return depth; }
    int getColorType() const { return color_type; }
    std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> getPixelAt(const int &x, const int &y);
 
private:
    void release();
 
    char *data;
    int width;
    int height;
    int depth;
    int color_type;

    png_size_t cols;
};
