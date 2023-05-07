#include <iostream>
#include <string>

#include <StronkImage.h>

using namespace StronkImage;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_image>" << std::endl;
        return 1;
    }

    std::string inputImagePath = argv[1];
    std::string outputEnergyMap = "energy_map.png";

    try
    {
        // Load input image
        StronkImage::Image inputImage(inputImagePath);

        // Generate energy map
        StronkImage::ImageData energyMap = StronkImage::Filter::generateEnergyMap(inputImage.getRawImageData());

        // Write energy map to disk
        StronkImage::Image energyMapImage;
        energyMapImage.setRawImageData(energyMap);
        energyMapImage.writeToFile(outputEnergyMap);
        inputImage.writeToFile(inputImagePath + "1.png");

        std::cout << "Successfully generated Sobel filter images and energy map." << std::endl;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
