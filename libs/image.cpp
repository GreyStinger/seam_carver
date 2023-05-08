#include <cmath>
#include <cstdio>


#include <Image.h>

// Definitions for Image
namespace StronkImage
{
	// Default constructor
	Image::Image()
	{
		// Initialize imageData with default values (width and height = 0)
		imageData = ImageData();
	}

	// Create image map from file
	Image::Image(const std::string &imageSpec)
	{
		// Load image from file
		loadFromFile(imageSpec);
	}

	Image::~Image()
	{
		// Destructor body
		// Since the Image class uses the ImageData class, which properly manages
		// its resources, no additional resource cleanup is required in the destructor.
	}

	ImageData &ImageData::operator=(const ImageData &other)
	{
		if (this != &other)
		{
			width = other.width;
			height = other.height;

			if (rgbPixelData)
			{
				delete[] rgbPixelData;
			}

			rgbPixelData = new RGBPixelBuf[width * height];

			for (unsigned int i = 0; i < width * height; ++i)
			{
				rgbPixelData[i] = other.rgbPixelData[i];
			}
		}
		return *this;
	}

	ImageData &Image::getRawImageData()
	{
		return imageData;
	}

	void Image::setRawImageData(const ImageData &newImageData)
	{
		imageData = newImageData;
	}
} // namespace StronkImage

// Definitions for ImageData
namespace StronkImage
{
	ImageData::ImageData()
		: height(0), width(0), rgbPixelData(nullptr) {}

	ImageData::ImageData(int width, int height)
		: height(height), width(width), rgbPixelData(nullptr)
	{
		if (height <= 0 || width <= 0)
		{
			throw std::invalid_argument("Invalid dimensions for the image");
		}

		rgbPixelData = new RGBPixelBuf[height * width];
	}

	ImageData::ImageData(const ImageData &other)
		: height(other.height), width(other.width), rgbPixelData(nullptr)
	{
		if (other.rgbPixelData)
		{
			rgbPixelData = new RGBPixelBuf[height * width];
			std::copy(other.rgbPixelData, other.rgbPixelData + height * width, rgbPixelData);
		}
	}

	ImageData::~ImageData()
	{
		delete[] rgbPixelData;
	}

	void ImageData::resizeBuffer(int newWidth, int newHeight)
	{
		if (newWidth <= 0 || newHeight <= 0)
		{
			throw std::invalid_argument("Invalid dimensions for resizing the image buffer");
		}

		if (newWidth == width && newHeight == height)
		{
			// No change in dimensions; do nothing
			return;
		}

		// Create a new buffer with the new dimensions
		RGBPixelBuf *newRgbPixelData = new RGBPixelBuf[newHeight * newWidth];

		// Copy the data from the old buffer to the new buffer
		int minWidth = std::min(width, (unsigned int)newWidth);
		int minHeight = std::min(height, (unsigned int)newHeight);
		for (int y = 0; y < minHeight; ++y)
		{
			for (int x = 0; x < minWidth; ++x)
			{
				newRgbPixelData[y * newWidth + x] = rgbPixelData[y * width + x];
			}
		}

		// Delete the old buffer and update the buffer pointer and dimensions
		delete[] rgbPixelData;
		rgbPixelData = newRgbPixelData;
		width = newWidth;
		height = newHeight;
	}

	RGBPixelBuf ImageData::getPixel(int x, int y) const
	{
		if (x < 0 || x >= width || y < 0 || y >= height)
		{
			throw std::out_of_range("Invalid pixel position");
		}

		return rgbPixelData[y * width + x];
	}

	void ImageData::setPixel(int x, int y, const RGBPixelBuf &pixel)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)
		{
			throw std::out_of_range("Invalid pixel position");
		}

		rgbPixelData[y * width + x] = pixel;
	}
} // namespace StronkImage