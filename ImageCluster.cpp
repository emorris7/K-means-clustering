#include <random>
#include <functional>
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
    //get the mean, must check counter as depending on intialization, might be no images assigned to cluster
    if (counter > 0)
    {
        for (int k = 0; k < size; k++)
        {
            cent[k] = cent[k] / counter;
        }
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

void MRREMI007::ImageCluster::makeClusters()
{
    if (numClusters > 0)
    {
        std::default_random_engine ranEngine;
        std::uniform_int_distribution<int> ranGenerator(0, numClusters - 1);
        auto randCLuster = std::bind(ranGenerator, ranEngine);
        //assign random cluster to each image for forgy intialization
        for (auto &i : images)
        {
            i.cluster = randCLuster();
        }
        int numChanges{1};
        while (numChanges != 0)
        {
            numChanges = 0;
            //make all the centroids for the clusters
            for (int i = 0; i < numClusters; i++)
            {
                makeCentroid(i);
            }
            //calculate distance between all images from all centroids and assign cluster numbers accordingly
            for (auto &i : images)
            {
                float minDistance{-1};
                int originalCluster{i.cluster};
                for (int j = 0; j < centroids.size(); j++)
                {
                    float distanceFrom = distance(i, centroids[j]);
                    if (minDistance < 0 || distanceFrom < minDistance)
                    {
                        minDistance = distanceFrom;
                        i.cluster = j;
                    }
                }
                if (i.cluster != originalCluster)
                {
                    numChanges++;
                }
            }
        }
    }
    else
    {
        std::cout << "Cannot make clusters. Invalid number of cluster: " << numClusters << std::endl;
    }
}
