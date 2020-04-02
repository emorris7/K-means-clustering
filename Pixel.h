#ifndef PIXEL
#define PIXEL

#include <iostream>

namespace MRREMI007
{
class Pixel
{
public:
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char grey;

    Pixel();

    Pixel(const unsigned char r, const unsigned char g, const unsigned char b);

    void calcGrey();

    friend std::ostream &operator<<(std::ostream &os, const Pixel &p);
};

std::ostream &operator<<(std::ostream &os, const Pixel &p);

} // namespace MRREMI007

#endif