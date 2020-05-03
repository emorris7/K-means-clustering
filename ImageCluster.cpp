#include <algorithm>
#include "ImageCluster.h"
#include "Image.h"
#include "dirent.h"
#include "math.h"
#include <iostream>
#include <fstream>

MRREMI007::ImageCluster::ImageCluster() {}

MRREMI007::ImageCluster::ImageCluster(const std::string directoryName, const int clusters, const int bin, const bool colourHist, const bool random)
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
        //directories with in directories
        if (fileName.front() != '.')
        {
            fileName = directoryName + "/" + file->d_name;
            Image imageFile(fileName, file->d_name, binSize, colourHist);
            images.push_back(imageFile);
        }
    }
    makeClusters(random);
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
    if (images.size() == 0)
    {
        std::cout << "Error: No images loaded to make centroids with" << std::endl;
    }
    else
    {
        std::vector<int> cent;
        //assuming all images have the same histogram dimensions
        int size = images[0].histogram.size();
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
        if (counter > 0 && centroids.size() > cluster)
        {
            for (int k = 0; k < size; k++)
            {
                cent[k] = cent[k] / counter;
            }
            centroids[cluster] = cent;
        }
    }
}

void MRREMI007::ImageCluster::makeClusters(const bool random)
{
    if (numClusters > 0)
    {
        if (random)
        {
            initializeRandomCentroids();
        }
        else
        {
            initializeCentroids();
        }

        int numChanges{1};
        while (numChanges != 0)
        {
            numChanges = 0;
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
            //make all the centroids for the clusters for next iteration, if there is going to be another iteration
            if (numChanges != 0)
            {
                for (int i = 0; i < numClusters; i++)
                {
                    makeCentroid(i);
                }
            }
        }
    }
    else
    {
        std::cout << "Cannot make clusters. Invalid number of cluster: " << numClusters << std::endl;
    }
}

void MRREMI007::ImageCluster::initializeCentroids()
{
    //choose random initial cluster
    std::srand(time(NULL));
    int i = rand() % images.size();
    //assign intial centroid
    centroids.push_back(images[i].histogram);

    //calculate other centroids
    while (centroids.size() < numClusters)
    {
        //find the distance from the closest centroid for all images
        for (auto &i : images)
        {
            float minDist{-1};
            //finding min distance
            for (auto &j : centroids)
            {
                float dist = distance(i, j);
                if (minDist < 0 || dist < minDist)
                {
                    minDist = dist;
                }
            }
            i.distance = minDist;
        }

        //find the image with the maximum minimum distance
        float maxDist{-1};
        Image newCentroid;
        for (auto &i : images)
        {
            if (i.distance > maxDist)
            {
                newCentroid = i;
                maxDist = i.distance;
            }
        }
        //the average pixel of this image is now the new centroid
        centroids.push_back(newCentroid.histogram);
    }
};

void MRREMI007::ImageCluster::initializeRandomCentroids()
{
    std::vector<int> randomClusters;
    std::srand(time(NULL));
    while (randomClusters.size() < numClusters)
    {
        int i = rand() % images.size();
        //check if the number of clusters is less than the number of images, else must allow duplicates
        if (randomClusters.size() < images.size())
        {
            if (std::find(randomClusters.begin(), randomClusters.end(), i) == randomClusters.end())
            {
                randomClusters.push_back(i);
            }
        }
        else
        {
            randomClusters.push_back(i);
        }
    }
    for (auto &i : randomClusters)
    {
        centroids.push_back(images[i].histogram);
    }
}

void MRREMI007::ImageCluster::printToFile(const std::string fileName)
{
    std::ofstream file;
    file.open(fileName);
    if (file.is_open())
    {
        file << *this;
        file.close();
    }
    else
    {
        std::cout << "Error: Unable to open " << fileName << std::endl;
    }
}

std::ostream &MRREMI007::operator<<(std::ostream &os, const ImageCluster &imageCluster)
{
    //assuming k clusters, numbered from 0 to k-1
    for (int i = 0; i < imageCluster.numClusters; i++)
    {
        os << "Cluster " << i << ": ";
        bool first{true};
        for (int j = 0; j < imageCluster.images.size(); j++)
        {
            if (imageCluster.images[j].cluster == i && first)
            {
                os << imageCluster.images[j];
                first = false;
            }
            else if (imageCluster.images[j].cluster == i)
            {
                os << ", " << imageCluster.images[j];
            }
            if (j == imageCluster.images.size() - 1)
            {
                os << "\n";
            }
        }
    }
    return os;
}
