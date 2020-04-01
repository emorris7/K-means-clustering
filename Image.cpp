#include "Image.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

MRREMI007::Image::Image()
{
    image = new std::vector<Pixel*>;
}

MRREMI007::Image::~Image()
{
    // for (auto &&v : *image)
    // {
    //     delete[] v;
    // }
    delete image;
}

MRREMI007::Image::Image(const std::string fileName, const int binSize)
{
}

bool MRREMI007::Image::readFromFile(const std::string fileName)
{
    std::ifstream inputFile(fileName, std::ios::binary);
    std::string output{};
    if (inputFile.is_open())
    {
        // remove first line "P6"
        std::getline(inputFile,output);
        // get next line to check for comments
        std::getline(inputFile,output);
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
        //gte rid of 255
        std::getline(inputFile, output);



        inputFile.close();
        return true;
    }
    return false;
}