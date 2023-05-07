#include <StronkImage.h>
#include <gtest/gtest.h>
#include <stdexcept>

using namespace StronkImage;

// Test ImageData default constructor
TEST(ImageDataTest, DefaultConstructor) {
    ImageData imageData;
    EXPECT_EQ(0, imageData.getWidth());
    EXPECT_EQ(0, imageData.getHeight());
}

// Test ImageData constructor with valid dimensions
TEST(ImageDataTest, ConstructorWithValidDimensions) {
    ImageData imageData(5, 5);
    EXPECT_EQ(5, imageData.getWidth());
    EXPECT_EQ(5, imageData.getHeight());
}

// Test ImageData constructor with invalid dimensions
TEST(ImageDataTest, ConstructorWithInvalidDimensions) {
    EXPECT_THROW(ImageData imageData(-1, 5), std::invalid_argument);
    EXPECT_THROW(ImageData imageData(5, -1), std::invalid_argument);
    EXPECT_THROW(ImageData imageData(0, 5), std::invalid_argument);
    EXPECT_THROW(ImageData imageData(5, 0), std::invalid_argument);
}

// Test resizeBuffer with valid dimensions
TEST(ImageDataTest, ResizeBufferValidDimensions) {
    ImageData imageData(5, 5);
    imageData.resizeBuffer(10, 10);
    EXPECT_EQ(10, imageData.getWidth());
    EXPECT_EQ(10, imageData.getHeight());
}

// Test resizeBuffer with invalid dimensions
TEST(ImageDataTest, ResizeBufferInvalidDimensions) {
    ImageData imageData(5, 5);
    EXPECT_THROW(imageData.resizeBuffer(-1, 10), std::invalid_argument);
    EXPECT_THROW(imageData.resizeBuffer(10, -1), std::invalid_argument);
    EXPECT_THROW(imageData.resizeBuffer(0, 10), std::invalid_argument);
    EXPECT_THROW(imageData.resizeBuffer(10, 0), std::invalid_argument);
}

// // Test ImageData copy constructor
// TEST(ImageDataTest, CopyConstructor) {
//     ImageData imageData1(5, 5);
//     RGBPixelBuf pixel = { 100, 150, 200, 255 };
//     imageData1.setPixel(1, 1, pixel);
//     ImageData imageData2(imageData1);
//     EXPECT_EQ(5, imageData2.getWidth());
//     EXPECT_EQ(5, imageData2.getHeight());
//     EXPECT_EQ(pixel, imageData2.getPixel(1, 1));
// }

// // Test getPixel and setPixel with valid positions
// TEST(ImageDataTest, GetSetPixelValidPositions) {
//     ImageData imageData(5, 5);
//     RGBPixelBuf pixel = { 100, 150, 200, 255 };
//     imageData.setPixel(1, 1, pixel);
//     EXPECT_EQ(pixel, imageData.getPixel(1, 1));
// }
