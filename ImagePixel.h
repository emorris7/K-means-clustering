#ifndef IMAGEPIXEL
#define IMAGEPIXEL

#include <string>
#include <vector>
#include "Pixel.h"
namespace MRREMI007
{
class ImagePixel
{
public:
    std::string imageName;
    int width;
    int height;
    std::vector<std::vector<Pixel>> image;
    int cluster;
    //k++
    float distance;
    Pixel averagePixel;

    ImagePixel();

    ~ImagePixel();

    ImagePixel(const std::string fileName, const std::string image);

    //copy constructor
    ImagePixel(const ImagePixel &other);

    //copy assignment
    ImagePixel &operator=(const ImagePixel &other);

    //move constructor
    ImagePixel(ImagePixel &&other);

    //move assignment
    ImagePixel &operator=(ImagePixel &&other);

    void readFromFile(const std::string fileName);

    void makeAveragePixel();

    friend std::ostream &operator<<(std::ostream &os, const ImagePixel image);
};

std::ostream &operator<<(std::ostream &os, const ImagePixel image);
} // namespace MRREMI007
#endif