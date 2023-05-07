#include <cmath>
#include <cstdio>

#include <jpeglib.h>
#include <png.h>

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

	void Image::loadFromFile(const std::string &imageSpec)
	{
		if (imageSpec.substr(imageSpec.find_last_of(".") + 1) == "jpg" || imageSpec.substr(imageSpec.find_last_of(".") + 1) == "jpeg")
		{
			// Load JPEG image using libjpeg
			FILE *infile = fopen(imageSpec.c_str(), "rb");
			if (!infile)
			{
				throw std::runtime_error("Error opening JPEG file");
			}

			jpeg_decompress_struct cinfo;
			jpeg_error_mgr jerr;

			cinfo.err = jpeg_std_error(&jerr);
			jpeg_create_decompress(&cinfo);
			jpeg_stdio_src(&cinfo, infile);
			jpeg_read_header(&cinfo, TRUE);
			jpeg_start_decompress(&cinfo);

			imageData.resizeBuffer(cinfo.output_width, cinfo.output_height);

			JSAMPARRAY buffer;
			buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, cinfo.output_width * cinfo.output_components, 1);

			while (cinfo.output_scanline < cinfo.output_height)
			{
				jpeg_read_scanlines(&cinfo, buffer, 1);

				for (unsigned int x = 0; x < cinfo.output_width; ++x)
				{
					RGBPixelBuf pixel;
					pixel.red = buffer[0][x * cinfo.output_components + 0];
					pixel.green = buffer[0][x * cinfo.output_components + 1];
					pixel.blue = buffer[0][x * cinfo.output_components + 2];
					pixel.opacity = 255;
					imageData.setPixel(x, cinfo.output_scanline - 1, pixel);
				}
			}

			jpeg_finish_decompress(&cinfo);
			jpeg_destroy_decompress(&cinfo);
			fclose(infile);
		}
		else if (imageSpec.substr(imageSpec.find_last_of(".") + 1) == "png")
		{
			// Load PNG image using libpng
			FILE *infile = fopen(imageSpec.c_str(), "rb");
			if (!infile)
			{
				throw std::runtime_error("Error opening PNG file");
			}

			png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			png_infop info_ptr = png_create_info_struct(png_ptr);

			if (setjmp(png_jmpbuf(png_ptr)))
			{
				png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
				fclose(infile);
				throw std::runtime_error("Error reading PNG file");
			}

			png_init_io(png_ptr, infile);
			png_read_info(png_ptr, info_ptr);

			unsigned int width = png_get_image_width(png_ptr, info_ptr);
			unsigned int height = png_get_image_height(png_ptr, info_ptr);
			imageData.resizeBuffer(width, height);

			// Check the color type and bit depth
			int color_type = png_get_color_type(png_ptr, info_ptr);
			int bit_depth = png_get_bit_depth(png_ptr, info_ptr);

			// Convert palette images to RGB
			if (color_type == PNG_COLOR_TYPE_PALETTE)
			{
				png_set_palette_to_rgb(png_ptr);
			}

			// Convert grayscale images to RGB
			if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
			{
				png_set_gray_to_rgb(png_ptr);
			}

			// Convert low bit-depth images to 8 bits
			if (bit_depth < 8)
			{
				png_set_packing(png_ptr);
			}

			// Expand 8-bit images to full 16 bits
			if (bit_depth == 16)
			{
				png_set_swap(png_ptr);
			}

			// Add an alpha channel if the image doesn't have one
			if (color_type & PNG_COLOR_MASK_ALPHA == 0)
			{
				png_set_add_alpha(png_ptr, 0xff, PNG_FILLER_AFTER);
			}

			png_read_update_info(png_ptr, info_ptr);

			png_bytepp row_pointers = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
			for (unsigned int y = 0; y < height; ++y)
			{
				row_pointers[y] = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
			}

			png_read_image(png_ptr, row_pointers);

			for (unsigned int y = 0; y < height; ++y)
			{
				for (unsigned int x = 0; x < width; ++x)
				{
					RGBPixelBuf pixel;
					png_bytep row = row_pointers[y];
					png_bytep px = &(row[x * 4]);
					pixel.red = px[0];
					pixel.green = px[1];
					pixel.blue = px[2];
					pixel.opacity = px[3];
					imageData.setPixel(x, y, pixel);
				}
			}

			for (unsigned int y = 0; y < height; ++y)
			{
				png_free(png_ptr, row_pointers[y]);
			}
			png_free(png_ptr, row_pointers);

			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			fclose(infile);
		}
		else
		{
			throw std::runtime_error("Unsupported file format");
		}
	}

	bool Image::writeToFile(const std::string &imageSpec)
	{
		if (imageSpec.substr(imageSpec.find_last_of(".") + 1) == "jpg" || imageSpec.substr(imageSpec.find_last_of(".") + 1) == "jpeg")
		{
			// Write JPEG image using libjpeg
			FILE *outfile = fopen(imageSpec.c_str(), "wb");
			if (!outfile)
			{
				throw std::runtime_error("Error opening JPEG file");
			}

			jpeg_compress_struct cinfo;
			jpeg_error_mgr jerr;

			cinfo.err = jpeg_std_error(&jerr);
			jpeg_create_compress(&cinfo);
			jpeg_stdio_dest(&cinfo, outfile);

			cinfo.image_width = imageData.width;
			cinfo.image_height = imageData.height;
			cinfo.input_components = 3;
			cinfo.in_color_space = JCS_RGB;

			jpeg_set_defaults(&cinfo);
			jpeg_set_quality(&cinfo, 90, TRUE); // Set quality: 0 (worst) to 100 (best)
			jpeg_start_compress(&cinfo, TRUE);

			JSAMPROW row_pointer[1];
			int row_stride = cinfo.image_width * 3;
			unsigned char *buffer = new unsigned char[row_stride];

			while (cinfo.next_scanline < cinfo.image_height)
			{
				for (unsigned int x = 0; x < cinfo.image_width; ++x)
				{
					RGBPixelBuf pixel = imageData.getPixel(x, cinfo.next_scanline);
					buffer[x * 3 + 0] = static_cast<unsigned char>(pixel.red);
					buffer[x * 3 + 1] = static_cast<unsigned char>(pixel.green);
					buffer[x * 3 + 2] = static_cast<unsigned char>(pixel.blue);
				}
				row_pointer[0] = buffer;
				jpeg_write_scanlines(&cinfo, row_pointer, 1);
			}

			delete[] buffer;

			jpeg_finish_compress(&cinfo);
			jpeg_destroy_compress(&cinfo);
			fclose(outfile);
		}
		else if (imageSpec.substr(imageSpec.find_last_of(".") + 1) == "png")
		{
			// Write PNG image using libpng
			FILE *outfile = fopen(imageSpec.c_str(), "wb");
			if (!outfile)
			{
				throw std::runtime_error("Error opening PNG file");
			}

			png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			png_infop info_ptr = png_create_info_struct(png_ptr);

			if (setjmp(png_jmpbuf(png_ptr)))
			{
				png_destroy_write_struct(&png_ptr, &info_ptr);
				fclose(outfile);
				throw std::runtime_error("Error writing PNG file");
			}

			png_init_io(png_ptr, outfile);

			png_set_IHDR(
				png_ptr, info_ptr,
				imageData.width, imageData.height,
				8, PNG_COLOR_TYPE_RGBA,
				PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

			png_write_info(png_ptr, info_ptr);

			png_bytepp row_pointers = (png_bytepp)png_malloc(png_ptr, imageData.height * sizeof(png_bytep));
			for (unsigned int y = 0; y < imageData.height; ++y)
			{
				row_pointers[y] = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
			}

			for (unsigned int y = 0; y < imageData.height; ++y)
			{
				for (unsigned int x = 0; x < imageData.width; ++x)
				{
					RGBPixelBuf pixel = imageData.getPixel(x, y);
					png_bytep row = row_pointers[y];
					png_bytep px = &(row[x * 4]);
					px[0] = pixel.red;
					px[1] = pixel.green;
					px[2] = pixel.blue;
					px[3] = pixel.opacity;
				}
			}

			png_write_image(png_ptr, row_pointers);
			png_write_end(png_ptr, NULL);

			for (unsigned int y = 0; y < imageData.height; ++y)
			{
				png_free(png_ptr, row_pointers[y]);
			}
			png_free(png_ptr, row_pointers);

			png_destroy_write_struct(&png_ptr, &info_ptr);
			fclose(outfile);
		}

		else
		{
			throw std::runtime_error("Unsupported file format");
		}
		return true;
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