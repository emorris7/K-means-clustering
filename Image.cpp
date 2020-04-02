#include "Image.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

MRREMI007::Image::Image()
{
}

MRREMI007::Image::~Image()
{
    for (int i = 0; i < image.size(); i++)
    {
        delete[] image[i];
    }
}

MRREMI007::Image::Image(const std::string fileName, const int binSize)
{
    cluster = 0;
    imageName = fileName;
    readFromFile(fileName);
    makeHistogram(binSize);
}

bool MRREMI007::Image::readFromFile(const std::string fileName)
{

    std::ifstream inputFile(fileName, std::ios::binary);
    std::string output{};
    if (inputFile.is_open())
    {
        // remove first line "P6"
        std::getline(inputFile, output);
        // get next line to check for comments
        std::getline(inputFile, output);
        //remove comment lines
        while (output.front() == '#')
        {
            std::cout << output << std::endl;
            std::getline(inputFile, output);
        }
        //output will now have width and height
        std::stringstream dimensions(output);
        dimensions >> width;
        dimensions >> height;
        //get rid of 255
        std::getline(inputFile, output);

        //read in rgb data
        unsigned char *memblock;
        //does it have to be remade each time?
        memblock = new unsigned char[width * 3];
        for (int i = 0; i < height; i++)
        {
            inputFile.read((char *)memblock, width * 3);
            Pixel *array = new Pixel[width];
            image.push_back(array);
            for (int j = 0, k = 0; j < width * 3, k < width; j += 3, k++)
            {
                Pixel pixelVal(memblock[j], memblock[j + 1], memblock[j + 2]);
                image[i][k] = pixelVal;
                // std::cout << "i " << i << " k " << k << std::endl;
                // std::cout << image[i][k] << std::endl;
            }
            // if (i > 1)
            // {
            //     std::cout << "<<<<<<<<<<<<PREVIOUS ROW <<<<<<<<<< " << i - 2 << std::endl;
            //     for (int k = 0; k < width; k++)
            //     {
            //         std::cout << image[0][k]<< std::endl;
            //         std::cout << "i " << i - 2 << " k " << k << std::endl;
            //         std::cout << image[i - 2][k] << std::endl;
            //     }
            //     std::cout << "<<<<<<<<<<<<END<<<<<<<<<< " << std::endl;
            // }
            // delete[] memblock;
        }
        delete[] memblock;

        // std::cout << ">>>>>>>>>>>>>>>>>>>BREAK>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
        // for (int i = 0; i < height; i++)
        // {
        //     for (int j = 0; j < width; j++)
        //     {
        //         std::cout << "i " << i << " j " << j <<std::endl;
        //         std::cout<< image[i][j] << std::endl;
        //     }
        // }

        inputFile.close();
        return true;
    }
    return false;
}

bool MRREMI007::Image::makeHistogram(const int binSize)
{
    if (image.size() == 0)
    {
        return false;
    }
    int size = 256 / binSize;
    //zero the array to the appropriate size
    for (int i = 0; i < size; i++)
    {
        histogram.push_back(0);
    }

    //create the histogram
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            (histogram[int(image[i][j].grey) / binSize])++;
        }
    }
    return true;
}

std::ostream& MRREMI007::operator<<(std::ostream& os, const Image image){
    os << image.imageName;
    return os;
}