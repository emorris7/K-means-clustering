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
    std::vector<Pixel*> image;
    std::vector<int> histogram;
    int cluster;

    Image();
    
    ~Image();

    Image(const std::string fileName, const int binSize);

    bool readFromFile(const std::string fileName);

    bool makeHistogram(const int binSize);

    friend std::ostream& operator<<(std::ostream& os, const Image image);
};

std::ostream& operator<<(std::ostream& os, const Image image);
} // namespace MRREMI007
#endif