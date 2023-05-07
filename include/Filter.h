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

	class Filter
	{
	public:
		// Convolute the current sobel matrix over the provided
		// grayscale pixel data.
		// Warning changes data passed in
		static ImageData ConvoluteSobelMatrix(ImageData &sourceImage, int matrix[3][3]);

		// Changes the image passed in into a grayscale image
		static void genGrayscaleData(ImageData &colourImage);

		// Return energy map of an image as an ImageData structure
		static ImageData generateEnergyMap(ImageData &sourceImage);

		// Remove the desired number of seams from the source image
		// provided as required using a minimum cost matrix generated
		// with the energy map provided to find the minimum cost seam.
		static void removeSeams(ImageData &sourceImage, ImageData &energyMap, int numSeams);

		// Just color the seams red in the image
		// WARNING: Very Very Slow, implemented for demonstration purposes only
		// Need to implement it properly, tried and it went half alright, but it's late and I'll go
		// have another look ath the dynamic programming system for it again in the future
		// static void colorSeams(ImageData &sourceImage, ImageData &energyMap, int numSeams);


		// Experimental:
		// Takes newly filtered image and original un-seamCut image and fills in a new image,
		// replacing the cut pixels with red pixels and rebuilding the original image but now
		// the seams that were cut will be replaced with red.
		// Same as above function implementation
		// static void reAddSeams(ImageData &newSeamCutImage, ImageData &OrigImage);
	};
}

#endif