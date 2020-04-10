#include "Image.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

MRREMI007::Image::Image()
{
}

MRREMI007::Image::~Image() {}

MRREMI007::Image::Image(const std::string fileName, const std::string image, const int binSize, const bool colourHist)
{
    cluster = -1;
    imageName = image;
    readFromFile(fileName);
    if (colourHist)
    {
        makeHistogramColour(binSize);
    }
    else
    {
        makeHistogram(binSize);
    }
}

//copy constructor
MRREMI007::Image::Image(const Image &other)
{
    imageName = other.imageName;
    width = other.width;
    height = other.height;
    histogram = other.histogram;
    cluster = other.cluster;

    for (auto &vec : other.image)
    {
        image.push_back(vec);
    }
}

//copy assignment
MRREMI007::Image &MRREMI007::Image::operator=(const Image &other)
{
    imageName = other.imageName;
    width = other.width;
    height = other.height;
    histogram = other.histogram;
    cluster = other.cluster;

    for (auto &vec : other.image)
    {
        image.push_back(vec);
    }
    return *this;
}

//move constructor
MRREMI007::Image::Image(Image &&other)
{
    imageName = other.imageName;
    width = other.width;
    height = other.height;
    histogram = other.histogram;
    cluster = other.cluster;

    for (auto &vec : other.image)
    {
        image.push_back(vec);
    }
}

//move assignment
MRREMI007::Image &MRREMI007::Image::operator=(Image &&other)
{
    if (this != &other)
    {
        imageName = other.imageName;
        width = other.width;
        height = other.height;
        histogram = other.histogram;
        cluster = other.cluster;

        for (auto &vec : other.image)
        {
            image.push_back(vec);
        }
    }
    return *this;
}

void MRREMI007::Image::readFromFile(const std::string fileName)
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
            // std::cout << output << std::endl;
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
            std::vector<Pixel> array;
            for (int j = 0, k = 0; j < width * 3, k < width; j += 3, k++)
            {
                Pixel pixelVal(memblock[j], memblock[j + 1], memblock[j + 2]);
                array.push_back(pixelVal);
                // std::cout << "i " << i << " k " << k << std::endl;
                // std::cout << image[i][k] << std::endl;
            }
            image.push_back(array);
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
    }
    else
    {
        std::cout << "Failed to open file: " << fileName << std::endl;
    }
}

void MRREMI007::Image::makeHistogram(const int binSize)
{
    if (image.size() == 0 || image.size() != height)
    {
        std::cout << "Cannot make histogram: no image loaded" << std::endl;
    }
    else
    {
        int size{};
        //check if binsize is an even divisor of the number of values for an usigned char
        if (256 % binSize != 0)
        {
            size = 256 / binSize + 1;
        }
        else
        {
            size = 256 / binSize;
        }
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
    }
}

void MRREMI007::Image::makeHistogramColour(const int binSize)
{
    if (image.size() == 0 || image.size() != height)
    {
        std::cout << "Cannot make histogram: no image loaded" << std::endl;
    }
    else
    {
        int size{};
        //check if binsize is an even divisor of the number of values for an usigned char
        if (256 % binSize != 0)
        {
            size = 256 / binSize + 1;
        }
        else
        {
            size = 256 / binSize;
        }
        //zero the array to the appropriate size, 3 times the size of the necessary array to store red, green and blue histograms back to back
        for (int i = 0; i < size * 3; i++)
        {
            histogram.push_back(0);
        }

        //create the histogram
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                //increment red
                (histogram[int(image[i][j].red) / binSize])++;
                //increment green
                (histogram[(int(image[i][j].green) / binSize) + size])++;
                //increment blue
                (histogram[(int(image[i][j].blue) / binSize) + size * 2])++;
            }
        }
    }
}

std::ostream &MRREMI007::operator<<(std::ostream &os, const Image image)
{
    os << image.imageName;
    return os;
}