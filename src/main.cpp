#include <iostream>
#include <string>
#include "StronkImage.h"

using namespace StronkImage;

void stripImage(const std::string& inputImagePath, const std::string& outputImagePath, int numSeams)
{
    // Load the input image
    Image inputImage(inputImagePath);

    // Generate a grayscale version of the image
    ImageData grayscaleImage = inputImage.getRawImageData();
    Filter::genGrayscaleData(grayscaleImage);

    // Generate an energy map of the grayscale image
    ImageData energyMap = Filter::generateEnergyMap(grayscaleImage);

    // Remove the specified number of seams from the input image
    Filter::colorSeams(inputImage.getRawImageData(), energyMap, numSeams);

    // Save the modified image to the output path
    inputImage.writeToFile(outputImagePath);
}

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <inputImagePath> <outputImagePath> <numSeams>" << std::endl;
        return 1;
    }

    std::string inputImagePath = argv[1];
    std::string outputImagePath = argv[2];
    int numSeams = std::stoi(argv[3]);

    try
    {
        stripImage(inputImagePath, outputImagePath, numSeams);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Successfully removed " << numSeams << " seams from " << inputImagePath << " and saved the result to " << outputImagePath << std::endl;

    return 0;
}
