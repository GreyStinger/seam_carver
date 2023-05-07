#pragma once
#ifndef STRONKIMAGE_IMAGE
#define STRONKIMAGE_IMAGE

#include <string>
#include <stdexcept>

#include <Pixel.h>

namespace StronkImage
{
	class ImageData
	{
	public:
		unsigned int width, height;

		// 1D array of pixels allocated for the height * width of the image
		RGBPixelBuf *rgbPixelData;

		// Default constructor that will generate an image with
		//     width and height of 0
		ImageData();

		// Allocate image with given height and width
		ImageData(int width, int height);

		// Copy constructor
		ImageData(const ImageData &other);

		// Destructor
		~ImageData();

		// Change buffer size for new height and width
		void resizeBuffer(unsigned int newWidth, unsigned int newHeight);

		// Get pixel at (x, y) position
		RGBPixelBuf getPixel(int x, int y) const;

		// Set pixel at (x, y) position
		void setPixel(int x, int y, const RGBPixelBuf &pixel);
	};
}

namespace StronkImage
{
	class Image
	{
	private:
		// Private image's image data
		ImageData imageData;

	public:
		// Default constructor
		Image(void);

		// Create image map from file
		Image(const std::string &imageSpec);

		// Default destructor
		virtual ~Image();

		// Load image from jpeg or png
		void loadFromFile(const std::string &imageSpec);

		// Write to file as jpeg or png
		bool writeToFile(const std::string &imageSpec);

		// Return a raw reference to the local ImageData structure
		ImageData &getRawData();
	};
}

#endif
