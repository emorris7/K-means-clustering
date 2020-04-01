#include "Pixel.h"

MRREMI007::Pixel::Pixel()
{
    red = 0;
    green = 0;
    blue = 0;
    grey = 0;
}

MRREMI007::Pixel::Pixel(const unsigned char r, const unsigned char g, const unsigned char b)
{
    red = r;
    green = g;
    blue = b;
    grey = 0;
}

void MRREMI007::Pixel::calcGrey(){
    grey = 0.21*red + 0.72*green + 0.07*blue;
}
