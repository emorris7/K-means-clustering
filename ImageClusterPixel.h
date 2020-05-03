#ifndef IMAGECLUSTERPIXEL
#define IMAGECLUSTERPIXEL

#include <vector>
#include "ImagePixel.h"
#include "Pixel.h"

namespace MRREMI007
{
class ImageClusterPixel
{
public:
    std::vector<ImagePixel> images;
    int numClusters;
    std::vector<Pixel> centroids;

    ImageClusterPixel();

    ImageClusterPixel(const std::string directoryName, const int clusters, const bool random);

    ~ImageClusterPixel();

    float distance(const ImagePixel &image, const Pixel &centroid);

    void makeCentroid(const int clusterNum);
    
    void makeClusters(const bool random);

    //kmeans++ method
    void initializeCentroids();

    void initializeRandomCentroids();

    void printToFile(const std::string fileName);

    friend std::ostream &operator<<(std::ostream &os, const ImageClusterPixel &cluster);
};

std::ostream &operator<<(std::ostream &os, const ImageClusterPixel &cluster);
} // namespace MRREMI007
#endif