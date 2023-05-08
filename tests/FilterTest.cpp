#include <StronkImage.h>
#include <gtest/gtest.h>
#include <unistd.h>

#define PATH_MAX 2048

using namespace StronkImage;

TEST(GenerateGaussianKernelTest, TestKernelSizeOdd) {
    int kernelSize = 5;
    float sigma = 1.0f;
    auto kernel = generateGaussianKernel(kernelSize, sigma);

    ASSERT_EQ(kernelSize, kernel.size());
    for (const auto& row : kernel) {
        ASSERT_EQ(kernelSize, row.size());
    }
}

TEST(GenerateGaussianKernelTest, TestKernelSizeEven) {
    int kernelSize = 4;
    float sigma = 1.0f;
    auto kernel = generateGaussianKernel(kernelSize, sigma);

    ASSERT_EQ(kernelSize, kernel.size());
    for (const auto& row : kernel) {
        ASSERT_EQ(kernelSize, row.size());
    }
}

TEST(GenerateGaussianKernelTest, TestKernelSum) {
    int kernelSize = 5;
    float sigma = 1.0f;
    auto kernel = generateGaussianKernel(kernelSize, sigma);

    float sum = 0.0f;
    for (const auto& row : kernel) {
        for (const auto& value : row) {
            sum += value;
        }
    }

    ASSERT_NEAR(1.0f, sum, 0.0001f);
}

TEST(GenerateGaussianKernelTest, TestKernelValues) {
    int kernelSize = 3;
    float sigma = 1.0f;
    auto kernel = generateGaussianKernel(kernelSize, sigma);

    std::vector<std::vector<float>> expectedKernel = {
        {0.07511361f, 0.12384140f, 0.07511361f},
        {0.12384140f, 0.20417996f, 0.12384140f},
        {0.07511361f, 0.12384140f, 0.07511361f}
    };

    for (int y = 0; y < kernelSize; ++y) {
        for (int x = 0; x < kernelSize; ++x) {
            ASSERT_NEAR(expectedKernel[y][x], kernel[y][x], 0.0001f);
        }
    }
}

TEST(FilterTest, GaussianBlurIdentity) {
    StronkImage::ImageData sourceImage(3, 3);
    sourceImage.setPixel(0, 0, {0, 0, 0, 255});
    sourceImage.setPixel(1, 0, {255, 255, 255, 255});
    sourceImage.setPixel(2, 0, {0, 0, 0, 255});
    sourceImage.setPixel(0, 1, {255, 255, 255, 255});
    sourceImage.setPixel(1, 1, {0, 0, 0, 255});
    sourceImage.setPixel(2, 1, {255, 255, 255, 255});
    sourceImage.setPixel(0, 2, {0, 0, 0, 255});
    sourceImage.setPixel(1, 2, {255, 255, 255, 255});
    sourceImage.setPixel(2, 2, {0, 0, 0, 255});

    StronkImage::ImageData originalImage = sourceImage;

    Filter::gaussianBlur(sourceImage, 0.0f);

    for (int y = 0; y < sourceImage.getHeight(); ++y) {
        for (int x = 0; x < sourceImage.getWidth(); ++x) {
            ASSERT_EQ(originalImage.getPixel(x, y), sourceImage.getPixel(x, y));
        }
    }
}

TEST(FilterTest, GaussianBlurUniformImage) {
    StronkImage::ImageData sourceImage(3, 3);
    for (int y = 0; y < sourceImage.getHeight(); ++y) {
        for (int x = 0; x < sourceImage.getWidth(); ++x) {
            sourceImage.setPixel(x, y, {128, 128, 128, 255});
        }
    }

    StronkImage::ImageData originalImage = sourceImage;

    Filter::gaussianBlur(sourceImage, 1.0f);

    for (int y = 0; y < sourceImage.getHeight(); ++y) {
        for (int x = 0; x < sourceImage.getWidth(); ++x) {
            ASSERT_EQ(originalImage.getPixel(x, y), sourceImage.getPixel(x, y));
        }
    }
}

TEST(FilterRemoveSeamsTest, RemoveOneSeam)
{
    Image inputImage("../input.jpg");
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
    Image inputImage("../image.jpg");
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
    Image inputImage("../input.jpg");
    ImageData sourceImage = inputImage.getRawImageData();
    ImageData grayscaleImage = sourceImage;
    Filter::genGrayscaleData(grayscaleImage);
    ImageData energyMap = Filter::generateEnergyMap(grayscaleImage);

    int initialWidth = sourceImage.getWidth();

    Filter::removeSeams(sourceImage, energyMap, 0);

    ASSERT_EQ(initialWidth, sourceImage.getWidth());
    ASSERT_EQ(energyMap.getWidth(), sourceImage.getWidth());
}
