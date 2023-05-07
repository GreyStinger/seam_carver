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
	    {-1, 0, 1}
	};

	// SobelMatrixY for vertical matrix calculation
	static int sobelMatrixY[3][3] = {
		{-1, -2, -1},
		{ 0,  0,  0},
		{ 1,  2,  1}
	};

	class Filter
	{
	private:
		// Convolute the current sobel matrix over the provided
		// grayscale pixel data.
		// Warning changes data passed in
		static void ConvoluteSobelMatrix(ImageData& sourceImage, int matrix[3][3]);

	public:
		// Changes the image passed in into a grayscale image
		static void genGrayscaleData(ImageData& colourImage);

		// Return energy map of an image as an ImageData structure
		static ImageData generateEnergyMap(ImageData& sourceImage);
	};
}

#endif