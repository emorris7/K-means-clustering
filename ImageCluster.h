#ifndef IMAGECLUSTER
#define IMAGECLUSTER

#include <vector>
#include "Image.h"

namespace MRREMI007
{
class ImageCluster
{
public:
    std::vector<Image> images;
    int numClusters;
    int binSize;
    std::vector<std::vector<int>> centroids;

    ImageCluster();

    ImageCluster(const std::string directoryName, const int clusters, const int bin, const bool colourHist);

    ~ImageCluster();

    float distance(const Image &image, const std::vector<int> &centroid);

    void makeCentroid(const int clusterNum);

    //forgy method
    void makeClusters();

    void initializeCentroids();

    void printToFile(const std::string fileName);

    friend std::ostream &operator<<(std::ostream &os, const ImageCluster &cluster);
};

std::ostream &operator<<(std::ostream &os, const ImageCluster &cluster);
} // namespace MRREMI007
#endif