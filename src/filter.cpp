#include <math.h>

#include <Filter.h>

namespace StronkImage
{
    void Filter::ConvoluteSobelMatrix(ImageData &sourceImage, int matrix[3][3])
    {
        // Get the dimensions of the source image
        int width = sourceImage.width;
        int height = sourceImage.height;

        // Create a temporary image to store the convoluted data
        ImageData tempImage(width, height);

        // Iterate through each pixel in the source image
        for (int y = 1; y < height - 1; ++y)
        {
            for (int x = 1; x < width - 1; ++x)
            {
                int sum = 0;

                // Apply the convolution matrix
                for (int j = -1; j <= 1; ++j)
                {
                    for (int i = -1; i <= 1; ++i)
                    {
                        int neighborX = x + i;
                        int neighborY = y + j;

                        // Accumulate the sum of the matrix convolution
                        sum += sourceImage.getPixel(neighborX, neighborY).red * matrix[j + 1][i + 1];
                    }
                }

                // Calculate the final pixel value by clamping the sum to [0, 255]
                int finalPixelValue = std::min(std::max(sum, 0), 255);

                // Set the grayscale value in the temporary image
                tempImage.setPixel(x, y, {static_cast<Quantum>(finalPixelValue), static_cast<Quantum>(finalPixelValue), static_cast<Quantum>(finalPixelValue), 255});
            }
        }

        // Copy the temporary image data back into the source image
        sourceImage = tempImage;
    }

    static void genGrayscaleData(ImageData &colourImage)
    {
        for (int y = 0; y < colourImage.height; ++y)
        {
            for (int x = 0; x < colourImage.width; ++x)
            {
                // Get the current pixel
                RGBPixelBuf pixel = colourImage.getPixel(x, y);

                // Calculate the grayscale value using the luminance formula:
                // Y = 0.299 * R + 0.587 * G + 0.114 * B
                Quantum grayValue = static_cast<Quantum>(0.299 * pixel.red + 0.587 * pixel.green + 0.114 * pixel.blue);

                // Set the grayscale value to the red, green, and blue channels
                pixel.red = grayValue;
                pixel.green = grayValue;
                pixel.blue = grayValue;

                // Update the pixel in the image
                colourImage.setPixel(x, y, pixel);
            }
        }
    }

    ImageData Filter::generateEnergyMap(ImageData &sourceImage)
    {
        // Create grayscale copy of the source image
        ImageData grayscaleImage = sourceImage;
        // Convert the grayscaleImage to actual grayscale
        // Assume genGrayscaleData() is a static method
        Filter::genGrayscaleData(grayscaleImage);

        // Apply Sobel filters
        ImageData sobelXImage = grayscaleImage;
        ImageData sobelYImage = grayscaleImage;
        ConvoluteSobelMatrix(sobelXImage, sobelMatrixX);
        ConvoluteSobelMatrix(sobelYImage, sobelMatrixY);

        // Calculate energy map
        ImageData energyMap(sourceImage.width, sourceImage.height);
        for (int y = 0; y < sourceImage.height; ++y)
        {
            for (int x = 0; x < sourceImage.width; ++x)
            {
                // Get the corresponding Sobel values for the current pixel
                int sobelX = sobelXImage.getPixel(x, y).red;
                int sobelY = sobelYImage.getPixel(x, y).red;

                // Calculate the energy value (gradient magnitude)
                int energy = static_cast<int>(std::sqrt(sobelX * sobelX + sobelY * sobelY));

                // Clamp the energy value to the range [0, 255]
                energy = std::min(std::max(energy, 0), 255);

                // Set the energy value to the energy map
                energyMap.setPixel(x, y, {static_cast<Quantum>(energy), static_cast<Quantum>(energy), static_cast<Quantum>(energy), 255});
            }
        }

        return energyMap;
    }
}
