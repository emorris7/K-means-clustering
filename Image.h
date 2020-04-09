#ifndef IMAGE
#define IMAGE

#include <string>
#include <vector>
#include "Pixel.h"
namespace MRREMI007
{
class Image
{
public:
    std::string imageName;
    int width;
    int height;
    std::vector<std::vector<Pixel>> image;
    std::vector<int> histogram;
    int cluster;

    Image();

    ~Image();

    Image(const std::string fileName, const std::string image, const int binSize);

    //copy constructor
    Image(const Image &other);

    //copy assignment
    Image &operator=(const Image &other);

    //move constructor
    Image(Image &&other);

    //move assignment
    Image &operator=(Image &&other);

    void readFromFile(const std::string fileName);

    void makeHistogram(const int binSize);

    void makeHistogramColour(const int binSize);

    friend std::ostream &operator<<(std::ostream &os, const Image image);
};

std::ostream &operator<<(std::ostream &os, const Image image);
} // namespace MRREMI007
#endif