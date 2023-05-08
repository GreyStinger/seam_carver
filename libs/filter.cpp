#include <cmath>
#include <vector>
#include <limits>
#include <algorithm>

#include <Filter.h>

namespace StronkImage
{
    // Helper function to generate a Gaussian kernel
    std::vector<std::vector<float>> generateGaussianKernel(int kernelSize, float sigma)
    {
        std::vector<std::vector<float>> kernel(kernelSize, std::vector<float>(kernelSize));
        float sum = 0.0f;
        int center = kernelSize / 2;

        for (int y = 0; y < kernelSize; ++y)
        {
            for (int x = 0; x < kernelSize; ++x)
            {
                float exponent = -((x - center) * (x - center) + (y - center) * (y - center)) / (2 * sigma * sigma);
                kernel[y][x] = std::exp(exponent) / (2 * M_PI * sigma * sigma);
                sum += kernel[y][x];
            }
        }

        // Normalize the kernel
        for (int y = 0; y < kernelSize; ++y)
        {
            for (int x = 0; x < kernelSize; ++x)
            {
                kernel[y][x] /= sum;
            }
        }

        return kernel;
    }

    void Filter::gaussianBlur(ImageData &sourceImage, float sigmaValue)
    {
        if (sigmaValue == 0.0f)
        {
            return; // Skip the processing if sigma value is 0.0
        }

        // Generate the Gaussian kernel
        int kernelSize = static_cast<int>(std::ceil(6 * sigmaValue)) | 1; // Ensure odd kernel size
        std::vector<std::vector<float>> kernel = generateGaussianKernel(kernelSize, sigmaValue);

        // Create a temporary image to store the convoluted data
        ImageData tempImage(sourceImage.getWidth(), sourceImage.getHeight());

        // Iterate through each pixel in the source image
        int center = kernelSize / 2;
        for (int y = 0; y < sourceImage.getHeight(); ++y)
        {
            for (int x = 0; x < sourceImage.getWidth(); ++x)
            {
                float sumRed = 0.0f, sumGreen = 0.0f, sumBlue = 0.0f;

                // Apply the convolution
                for (int j = -center; j <= center; ++j)
                {
                    for (int i = -center; i <= center; ++i)
                    {
                        int neighborX = std::clamp(x + i, 0, static_cast<int>(sourceImage.getWidth()) - 1);
                        int neighborY = std::clamp(y + j, 0, static_cast<int>(sourceImage.getHeight()) - 1);

                        // Accumulate the sum of the convolution
                        RGBPixelBuf pixel = sourceImage.getPixel(neighborX, neighborY);
                        float kernelValue = kernel[j + center][i + center];
                        sumRed += pixel.red * kernelValue;
                        sumGreen += pixel.green * kernelValue;
                        sumBlue += pixel.blue * kernelValue;
                    }
                }

                // Set the pixel value in the temporary image
                tempImage.setPixel(x, y, {static_cast<Quantum>(sumRed), static_cast<Quantum>(sumGreen), static_cast<Quantum>(sumBlue), 255});
            }
        }

        // Copy the temporary image data back into the source image
        sourceImage = tempImage;
    }

    ImageData Filter::ConvoluteSobelMatrix(ImageData &sourceImage, int matrix[3][3])
    {
        // Get the dimensions of the source image
        int width = sourceImage.width;
        int height = sourceImage.height;

        // Create a temporary image to store the convoluted data
        ImageData tempImage(width, height);

        // Iterate through each pixel in the source image
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                int sum = 0;

                // Apply the convolution matrix
                for (int j = -1; j <= 1; ++j)
                {
                    for (int i = -1; i <= 1; ++i)
                    {
                        int neighborX = std::clamp(x + i, 0, width - 1);
                        int neighborY = std::clamp(y + j, 0, height - 1);

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
        return tempImage;
    }

    void Filter::genGrayscaleData(ImageData &colourImage)
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
        ImageData sobelXImage = ConvoluteSobelMatrix(grayscaleImage, sobelMatrixX);
        ImageData sobelYImage = ConvoluteSobelMatrix(grayscaleImage, sobelMatrixY);

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

    void Filter::removeSeams(ImageData &sourceImage, ImageData &energyMap, int numSeams)
    {
        for (int seamCount = 0; seamCount < numSeams; ++seamCount)
        {
            // Step 2: Initialize a minimum path energy 2D array
            ImageData minPathEnergy(sourceImage.getWidth(), sourceImage.getHeight());

            // Step 3: Iterate through each pixel in the energy map
            for (unsigned int y = 1; y < sourceImage.getHeight() - 1; ++y)
            {
                for (unsigned int x = 1; x < sourceImage.getWidth() - 1; ++x)
                {
                    int currentEnergy = energyMap.getPixel(x, y).red;
                    int minEnergy = minPathEnergy.getPixel(x, y - 1).red;

                    if (x > 1)
                    {
                        int leftEnergy = minPathEnergy.getPixel(x - 1, y - 1).red;
                        minEnergy = std::min(minEnergy, leftEnergy);
                    }

                    if (x < sourceImage.getWidth() - 2)
                    {
                        int rightEnergy = minPathEnergy.getPixel(x + 1, y - 1).red;
                        minEnergy = std::min(minEnergy, rightEnergy);
                    }

                    minPathEnergy.setPixel(x, y, RGBPixelBuf{(unsigned int)currentEnergy + minEnergy, 0, 0, 0});
                }
            }

            // Step 4: Find the pixel with the minimum energy value in the bottom row
            int minIdx = 1;
            int minEnergy = minPathEnergy.getPixel(1, sourceImage.getHeight() - 2).red;
            for (unsigned int x = 2; x < sourceImage.getWidth() - 1; ++x)
            {
                int currentEnergy = minPathEnergy.getPixel(x, sourceImage.getHeight() - 2).red;
                if (currentEnergy < minEnergy)
                {
                    minEnergy = currentEnergy;
                    minIdx = x;
                }
            }

            // Step 5: Trace back the lowest energy seam
            std::vector<int> seam(sourceImage.getHeight());
            seam[sourceImage.getHeight() - 1] = minIdx;

            for (int y = sourceImage.getHeight() - 2; y > 0; --y)
            {
                int minEnergy = minPathEnergy.getPixel(seam[y + 1], y).red;
                seam[y] = seam[y + 1];

                if (seam[y + 1] > 1)
                {
                    int leftEnergy = minPathEnergy.getPixel(seam[y + 1] - 1, y).red;
                    if (leftEnergy < minEnergy)
                    {
                        minEnergy = leftEnergy;
                        seam[y] = seam[y + 1] - 1;
                    }
                }

                if (seam[y + 1] < sourceImage.getWidth() - 2)
                {
                    int rightEnergy = minPathEnergy.getPixel(seam[y + 1] + 1, y).red;
                    if (rightEnergy < minEnergy)
                    {
                        seam[y] = seam[y + 1] + 1;
                    }
                }
            }

            ImageData newImage(sourceImage.getWidth() - 1, sourceImage.getHeight());
            ImageData newEnergyMap(energyMap.getWidth() - 1, energyMap.getHeight());
            for (unsigned int y = 0; y < sourceImage.getHeight(); ++y)
            {
                for (unsigned int x = 0; x < sourceImage.getWidth(); ++x)
                {
                    if (x < static_cast<unsigned int>(seam[y]))
                    {
                        newImage.setPixel(x, y, sourceImage.getPixel(x, y));
                        newEnergyMap.setPixel(x, y, energyMap.getPixel(x, y));
                    }
                    else if (x > static_cast<unsigned int>(seam[y]))
                    {
                        newImage.setPixel(x - 1, y, sourceImage.getPixel(x, y));
                        newEnergyMap.setPixel(x - 1, y, energyMap.getPixel(x, y));
                    }
                }
            }

            // Step 7: Update the source image and regenerate the energy map
            sourceImage = newImage;
            energyMap = newEnergyMap;
        }
    }
}
