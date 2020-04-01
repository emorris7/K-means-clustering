#ifndef PIXEL
#define PIXEL

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
};
} // namespace MRREMI007

#endif