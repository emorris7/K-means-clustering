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
    calcGrey();
}
MRREMI007::Pixel::~Pixel() {}

//copy constructor
MRREMI007::Pixel::Pixel(const Pixel &other)
{
    red = other.red;
    green = other.green;
    blue = other.blue;
    grey = other.grey;
}

//copy assignment
MRREMI007::Pixel &MRREMI007::Pixel::operator=(const Pixel &other)
{
    red = other.red;
    green = other.green;
    blue = other.blue;
    grey = other.grey;
    return *this;
}

//move constructor
MRREMI007::Pixel::Pixel(Pixel &&other)
{
    red = other.red;
    green = other.green;
    blue = other.blue;
    grey = other.grey;
}

//move assignment
MRREMI007::Pixel &MRREMI007::Pixel::operator=(Pixel &&other)
{
    if (this != &other)
    {
        red = other.red;
        green = other.green;
        blue = other.blue;
        grey = other.grey;
    }
    return *this;
}

void MRREMI007::Pixel::calcGrey()
{
    grey = 0.21 * red + 0.72 * green + 0.07 * blue;
}

std::ostream &MRREMI007::operator<<(std::ostream &os, const Pixel &p)
{
    os << "Red: " << std::to_string(p.red) << " Green: " << std::to_string(p.green) << " Blue: " << std::to_string(p.blue) << " Grey: " << std::to_string(p.grey);
    return os;
}
