#include "ImageCluster.h"
#include "ImageClusterPixel.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    int binSize{-1};
    bool colourHist{false};
    int numClusters{-1};
    std::string outFile{};
    bool usePixel{false};
    if (argc < 2)
    {
        std::cout << "Invalid run attempt, please supply directory path" << std::endl;
    }
    else
    {
        for (int i = 2; i < argc; i++)
        {
            if (std::string(argv[i]) == "-o")
            {
                outFile = argv[i + 1];
            }
            if (std::string(argv[i]) == "-k")
            {
                numClusters = std::stoi(argv[i + 1]);
            }
            if (std::string(argv[i]) == "-bin")
            {
                binSize = std::stoi(argv[i + 1]);
            }
            if (std::string(argv[i]) == "-colour")
            {
                colourHist = true;
            }
            if (std::string(argv[i]) == "-usePixel")
            {
                usePixel = true;
            }
        }
        //set default for cluster if it hasn't been specified
        if (numClusters < 0)
        {
            numClusters = 10;
        }
        //Do clustering using average pixel feature
        if (usePixel)
        {
            MRREMI007::ImageClusterPixel cluster(argv[1], numClusters);
            //no file name specified
            if (outFile == "")
            {
                std::cout << cluster << std::endl;
            }
            else
            {
                std::cout << "Writing clusters to file: " << outFile << std::endl;
                cluster.printToFile(outFile);
            }
        }
        //Do clustering using colour histogram or greyscale histogram
        else
        {
            //Set default bin size if it has not been specified
            if (binSize < 0)
            {
                binSize = 1;
            }
            MRREMI007::ImageCluster cluster(argv[1], numClusters, binSize, colourHist);
            //no file name specified
            if (outFile == "")
            {
                std::cout << cluster << std::endl;
            }
            else
            {
                std::cout << "Writing clusters to file: " << outFile << std::endl;
                cluster.printToFile(outFile);
            }
        }
    }
}
