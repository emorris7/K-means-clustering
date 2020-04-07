#include "ImageCluster.h"
#include "Image.h"
#include "dirent.h"
#include "math.h"

MRREMI007::ImageCluster::ImageCluster() {}

MRREMI007::ImageCluster::ImageCluster(const std::string directoryName, const int clusters, const int bin)
{
    numClusters = clusters;
    binSize = bin;
    //go through directory and make image objects
    DIR *directory;
    dirent *file;
    directory = opendir(directoryName.c_str());
    while (file = readdir(directory))
    {

        std::string fileName = file->d_name;
        //directories withint directories
        if (fileName.front() != '.')
        {
            fileName = directoryName + "/" + file->d_name;
            Image imageFile(fileName, binSize);
            images.push_back(imageFile);
        }
        else
        {
            std::cout << "Directory" << std::endl;
        }
    }
}

MRREMI007::ImageCluster::~ImageCluster() {}

float MRREMI007::ImageCluster::distance(const Image &image, const std::vector<int> &centroid)
{
    if (image.histogram.size() != centroid.size())
    {
        return -1;
    }
    float total{};
    for (int i = 0; i < centroid.size(); i++)
    {
        total += std::pow(image.histogram[i] - centroid[i], 2);
    }
    float distance = std::sqrt(total);
    return distance;
}

void MRREMI007::ImageCluster::makeCentroid(const int cluster)
{
    std::vector<int> cent;
    int size = 256 / binSize;
    //initialize a zero vector
    for (int i = 0; i < size; i++)
    {
        cent.push_back(0);
    }
    int counter{};
    //total all the histograms for the images in this cluster
    for (auto &image : images)
    {
        for (int j = 0; j < size; j++)
        {
            if (image.cluster == cluster)
            {
                cent[j] += image.histogram[j];
                if (j == 0)
                {
                    counter++;
                }
            }
        }
    }
    //get the mean
    for (int k = 0; k < size; k++)
    {
        cent[k] = cent[k] / counter;
    }
    //if centroid already exists for the cluster
    if (centroids.size() > cluster)
    {
        centroids[cluster] = cent;
    }
    //else just starting the algorithm, must add intial centroids to array
    else
    {
        std::cout << "Making centroid: " << cluster << std::endl;
        centroids.push_back(cent);
    }
}