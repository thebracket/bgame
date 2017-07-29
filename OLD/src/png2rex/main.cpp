#include <iostream>
#include <string>
#include "PngLoader.hpp"
#include "REXSpeeder.h"

using namespace xp;

constexpr int rex_version = -1;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "png2rex usage: png2rex <pngfile> <rexfile>\n";
        return 0;
    }
    std::string input_file = std::string(argv[1]);
    std::string output_file = std::string(argv[2]);

    std::cout << "Converting " << input_file << " to " << output_file << "\n";

    PngLoader png;
    png.load(input_file);

    const int width = png.getWidth();
    const int height = png.getHeight();
    std::cout << "Original image is " << width << "x" << height << " pixels\n";

    RexImage rex{rex_version, width, height, 1};
    for (int y=0; y<height; ++y) {
        for (int x=0; x<width; ++x) {
            uint8_t r,g,b,a;
            std::tie(r,g,b,a) = png.getPixelAt(x,(height-1)-y);
            if (a == 0) {
                rex.setTile(0, x, y, RexTile{0, 0, 0, 0, 255, 0, 255});
            } else if (a==255) {
                rex.setTile(0, x, y, RexTile{219, r, g, b, 0, 0, 0});
            } else {
                float percent = (float)a / 255.0F;
                if (percent < 0.25F) {
                    rex.setTile(0, x, y, RexTile{176, r, g, b, 0, 0, 0});
                } else if (percent < 0.5F) {
                    rex.setTile(0, x, y, RexTile{177, r, g, b, 0, 0, 0});
                } else if (percent < 0.75F) {
                    rex.setTile(0, x, y, RexTile{178, r, g, b, 0, 0, 0});
                } else {
                    rex.setTile(0, x, y, RexTile{219, r, g, b, 0, 0, 0});
                }
            }
        }
    }
    rex.flatten();
    rex.save(output_file.c_str());

    std::cout << "Done\n";

    return 0;
}