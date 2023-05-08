#pragma once
#ifndef STRONKIMAGE_FILTER
#define STRONKIMAGE_FILTER

#include <Image.h>

namespace StronkImage
{
	// SobelMatrixX for horizontal matrix calculation
	static int sobelMatrixX[3][3] = {
		{-1, 0, 1},
		{-2, 0, 2},
		{-1, 0, 1}};

	// SobelMatrixY for vertical matrix calculation
	static int sobelMatrixY[3][3] = {
		{-1, -2, -1},
		{0, 0, 0},
		{1, 2, 1}};

	/**
	 * @brief The Filter class provides various image filtering operations.
	 */
	class Filter
	{
	public:
		/**
		 * @brief Applies Gaussian blur filter to the input image using the default sigma value if sigmaValue is NULL.
		 *
		 * This function applies a Gaussian blur filter to the input image using the provided sigma value or the default
		 * sigma value of 1.0 if sigmaValue is NULL. The Gaussian blur filter helps to remove noise and smooth out the
		 * image. The function modifies the input ImageData object in place.
		 *
		 * @param sourceImage The input ImageData object to be blurred.
		 * @param sigmaValue The sigma value to be used for the Gaussian blur filter. If sigmaValue is NULL, the default
		 *                  sigma value of 1.0 is used.
		 */
		static void gaussianBlur(ImageData &sourceImage, float sigmaValue = 1.0);

		/**
		 * @brief Convolute the current sobel matrix over the provided grayscale pixel data.
		 *
		 * This function takes an ImageData object representing a grayscale image and a 3x3 Sobel filter matrix
		 * as input, and applies convolution to the image using the Sobel filter. The function returns a new
		 * ImageData object representing the convoluted image. This function modifies the input ImageData object
		 * in place.
		 *
		 * @param sourceImage The ImageData object representing the grayscale image to be convoluted.
		 * @param matrix The 3x3 Sobel filter matrix to be used for convolution.
		 * @return A new ImageData object representing the convoluted image.
		 */
		static ImageData ConvoluteSobelMatrix(ImageData &sourceImage, int matrix[3][3]);

		/**
		 * @brief Changes the image passed in into a grayscale image.
		 *
		 * This function takes an ImageData object representing a color image as input and converts it to grayscale.
		 * The function modifies the input ImageData object in place.
		 *
		 * @param colourImage The ImageData object representing the color image to be converted to grayscale.
		 */
		static void genGrayscaleData(ImageData &colourImage);

		/**
		 * @brief Returns energy map of an image as an ImageData structure.
		 *
		 * This function takes an ImageData object representing a grayscale image as input and computes an energy
		 * map of the image. The energy map represents the magnitude of the gradient at each pixel in the image.
		 * The function returns an ImageData object representing the energy map.
		 *
		 * @param sourceImage The ImageData object representing the grayscale image to be used for computing the energy map.
		 * @return An ImageData object representing the energy map.
		 */
		static ImageData generateEnergyMap(ImageData &sourceImage);

		/**
		 * @brief Remove the desired number of seams from the source image provided as required using a minimum
		 * cost matrix generated with the energy map provided to find the minimum cost seam.
		 *
		 * This function takes an ImageData object representing the source image, an ImageData object representing
		 * the energy map of the source image, and an integer value representing the number of seams to be removed
		 * from the image. The function removes the desired number of seams from the source image using a minimum
		 * cost matrix generated with the energy map to find the minimum cost seam. The function modifies the input
		 * ImageData object in place.
		 *
		 * @param sourceImage The ImageData object representing the source image.
		 * @param energyMap The ImageData object representing the energy map of the source image.
		 * @param numSeams The number of seams to be removed from the image.
		 */
		static void removeSeams(ImageData &sourceImage, ImageData &energyMap, int numSeams);
	};
}

#endif