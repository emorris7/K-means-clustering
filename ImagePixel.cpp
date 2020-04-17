#include "ImagePixel.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

MRREMI007::ImagePixel::ImagePixel()
{
}

MRREMI007::ImagePixel::~ImagePixel() {}

MRREMI007::ImagePixel::ImagePixel(const std::string fileName, const std::string image)
{
    cluster = -1;
    distance = -1;
    imageName = image;
    readFromFile(fileName);
    makeAveragePixel();
}

//copy constructor
MRREMI007::ImagePixel::ImagePixel(const ImagePixel &other)
{
    imageName = other.imageName;
    width = other.width;
    height = other.height;
    distance = other.distance;
    averagePixel = other.averagePixel;
    cluster = other.cluster;

    for (auto &vec : other.image)
    {
        image.push_back(vec);
    }
}

//copy assignment
MRREMI007::ImagePixel &MRREMI007::ImagePixel::operator=(const ImagePixel &other)
{
    imageName = other.imageName;
    width = other.width;
    height = other.height;
    distance = other.distance;
    averagePixel = other.averagePixel;
    cluster = other.cluster;

    for (auto &vec : other.image)
    {
        image.push_back(vec);
    }
    return *this;
}

//move constructor
MRREMI007::ImagePixel::ImagePixel(ImagePixel &&other)
{
    imageName = other.imageName;
    width = other.width;
    height = other.height;
    distance = other.distance;
    averagePixel = other.averagePixel;
    cluster = other.cluster;

    for (auto &vec : other.image)
    {
        image.push_back(vec);
    }
}

//move assignment
MRREMI007::ImagePixel &MRREMI007::ImagePixel::operator=(ImagePixel &&other)
{
    if (this != &other)
    {
        imageName = other.imageName;
        width = other.width;
        height = other.height;
        distance = other.distance;
        averagePixel = other.averagePixel;
        cluster = other.cluster;

        for (auto &vec : other.image)
        {
            image.push_back(vec);
        }
    }
    return *this;
}

void MRREMI007::ImagePixel::readFromFile(const std::string fileName)
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
            }
            image.push_back(array);
        }
        delete[] memblock;
        inputFile.close();
    }
    else
    {
        std::cout << "Failed to open file: " << fileName << std::endl;
    }
}

void MRREMI007::ImagePixel::makeAveragePixel()
{
    double red{};
    double green{};
    double blue{};
    int counter{};
    for (auto &i : image)
    {
        for (auto &j : i)
        {
            //skip black pixels
            if (j.red == 0 && j.green == 0 && j.blue == 0)
            {
                continue;
            }
            red += j.red;
            green += j.green;
            blue += j.blue;
            counter++;
        }
    }
    // unsigned char rVal = red / (width * height);
    // unsigned char gVal = green / (width * height);
    // unsigned char bVal = blue / (width * height);
    unsigned char rVal = red / counter;
    unsigned char gVal = green / counter;
    unsigned char bVal = blue / counter;
    // std::cout << "Total: " << width * height << " black: " << counter << std::endl;
    Pixel pixel(rVal, gVal, bVal);
    averagePixel = pixel;
}

std::ostream &MRREMI007::operator<<(std::ostream &os, const ImagePixel image)
{
    os << image.imageName;
    return os;
}