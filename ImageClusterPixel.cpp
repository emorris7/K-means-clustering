#include <algorithm>
#include "ImageClusterPixel.h"
#include "ImagePixel.h"
#include "dirent.h"
#include "math.h"
#include <iostream>
#include <fstream>

MRREMI007::ImageClusterPixel::ImageClusterPixel() {}

MRREMI007::ImageClusterPixel::ImageClusterPixel(const std::string directoryName, const int clusters, const bool random)
{
    numClusters = clusters;
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
            ImagePixel imageFile(fileName, file->d_name);
            images.push_back(imageFile);
        }
    }
    makeClusters(random);
}

MRREMI007::ImageClusterPixel::~ImageClusterPixel() {}

float MRREMI007::ImageClusterPixel::distance(const ImagePixel &image, const Pixel &centroid) const
{
    float total = std::pow(image.averagePixel.red - centroid.red, 2) + std::pow(image.averagePixel.green - centroid.green, 2) + std::pow(image.averagePixel.blue - centroid.blue, 2);
    float distance = std::sqrt(total);
    return distance;
}

void MRREMI007::ImageClusterPixel::makeCentroid(const int cluster)
{
    if (images.size() == 0)
    {
        std::cout << "Error: No images loaded to make centroids with" << std::endl;
    }
    else
    {
        double red{};
        double green{};
        double blue{};
        int counter{};
        for (auto &i : images)
        {
            if (i.cluster == cluster)
            {
                red += i.averagePixel.red;
                green += i.averagePixel.green;
                blue += i.averagePixel.blue;
                counter++;
            }
        }

        //check if there are any images in the cluster
        if (counter > 0 and cluster < centroids.size())
        {
            unsigned char rVal = red / counter;
            unsigned char gVal = green / counter;
            unsigned char bVal = blue / counter;
            centroids[cluster].red = rVal;
            centroids[cluster].green = gVal;
            centroids[cluster].blue = bVal;
        }
    }
}

void MRREMI007::ImageClusterPixel::makeClusters(const bool random)
{
    if (numClusters > 0)
    {
        if (random)
        {
            initializeRandomCentroids();
        }
        else
        {
            //k++
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

//kmeans++ intialization
void MRREMI007::ImageClusterPixel::initializeCentroids()
{
    //choose random intial cluster
    std::srand(time(NULL));
    int i = rand() % images.size();
    //assign intial centroid
    centroids.push_back(images[i].averagePixel);

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
        ImagePixel newCentroid;
        for (auto &i : images)
        {
            if (i.distance > maxDist)
            {
                newCentroid = i;
                maxDist = i.distance;
            }
        }
        //the average pixel of this image is now the new centroid
        centroids.push_back(newCentroid.averagePixel);
    }
}

void MRREMI007::ImageClusterPixel::initializeRandomCentroids()
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
        centroids.push_back(images[i].averagePixel);
    }
}

void MRREMI007::ImageClusterPixel::printToFile(const std::string fileName) const
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

std::ostream &MRREMI007::operator<<(std::ostream &os, const ImageClusterPixel &imageCluster)
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
