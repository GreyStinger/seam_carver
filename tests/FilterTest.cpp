#include <StronkImage.h>
#include <gtest/gtest.h>

using namespace StronkImage;

TEST(FilterRemoveSeamsTest, RemoveOneSeam)
{
    Image inputImage("../tests/test_images/input.jpg");
    ImageData sourceImage = inputImage.getRawImageData();
    ImageData grayscaleImage = sourceImage;
    Filter::genGrayscaleData(grayscaleImage);
    ImageData energyMap = Filter::generateEnergyMap(grayscaleImage);

    int initialWidth = sourceImage.getWidth();

    Filter::removeSeams(sourceImage, energyMap, 1);

    ASSERT_EQ(initialWidth - 1, sourceImage.getWidth());
    ASSERT_EQ(energyMap.getWidth(), sourceImage.getWidth());

    Image outputImage;
    outputImage.setRawImageData(sourceImage);
    outputImage.writeToFile("test_images/output_remove_one_seam.jpg");
}

TEST(FilterRemoveSeamsTest, RemoveMultipleSeams)
{
    Image inputImage("../tests/test_images/input.jpg");
    ImageData sourceImage = inputImage.getRawImageData();
    ImageData grayscaleImage = sourceImage;
    Filter::genGrayscaleData(grayscaleImage);
    ImageData energyMap = Filter::generateEnergyMap(grayscaleImage);

    int initialWidth = sourceImage.getWidth();
    int numSeamsToRemove = 10;

    Filter::removeSeams(sourceImage, energyMap, numSeamsToRemove);

    ASSERT_EQ(initialWidth - numSeamsToRemove, sourceImage.getWidth());
    ASSERT_EQ(energyMap.getWidth(), sourceImage.getWidth());

    Image outputImage;
    outputImage.setRawImageData(sourceImage);
    outputImage.writeToFile("test_images/output_remove_multiple_seams.jpg");
}

TEST(FilterRemoveSeamsTest, RemoveNoSeams)
{
    Image inputImage("../tests/test_images/input.jpg");
    ImageData sourceImage = inputImage.getRawImageData();
    ImageData grayscaleImage = sourceImage;
    Filter::genGrayscaleData(grayscaleImage);
    ImageData energyMap = Filter::generateEnergyMap(grayscaleImage);

    int initialWidth = sourceImage.getWidth();

    Filter::removeSeams(sourceImage, energyMap, 0);

    ASSERT_EQ(initialWidth, sourceImage.getWidth());
    ASSERT_EQ(energyMap.getWidth(), sourceImage.getWidth());
}
