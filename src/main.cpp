#include <Magick++.h>
#include <iostream>
#include <string>

#include <StronkImage.h>


// Define horizontal and vertical Sobel kernels
const int sobel_h[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
const int sobel_v[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};


Magick::Image filter_sobel(const Magick::Image& input, const int kernel[][3], bool is_gx);
void makeGrayscale(Magick::Image& image);

int main(int argc, char** argv)
{
    std::string filename ("image.jpg");

    Magick::InitializeMagick(*argv);
    
    // std::cout << "Please enter the filename: ";
    // std::cin >> filename;

    Magick::Image image (filename);
    makeGrayscale(image);

    Magick::Image gradx = filter_sobel(image, sobel_h, true);
    Magick::Image grady = filter_sobel(image, sobel_v, false);

    gradx.write("gradx.jpg");
    grady.write("grady.jpg");
    image.write("output.jpg");

    std::cout << "wrote input to: output.jpg" << std::endl;

    return 0;
}

void makeGrayscale(Magick::Image& image)
{
    image.type(Magick::GrayscaleType);
    image.depth(8);
    image.colorSpace(Magick::GRAYColorspace);
}


// Apply Sobel operator filter to input image
Magick::Image filter_sobel(const Magick::Image& input, const int kernel[][3], bool is_gx) 
{
    // Allocate output image
    Magick::Image output(input.size(), "white");

    // Get image dimensions
    size_t width = input.columns();
    size_t height = input.rows();

    // Allocate pixel buffer
    Magick::Image input_copy = input;
    Magick::PixelPacket *pixels = input_copy.getPixels(0, 0, width, height);
    std::cout << "Single pixel pointer size: " << sizeof(pixels) << "\n";
    std::cout << "Single dereferenced pixel size: " << sizeof(*pixels) << "\n";

    // Loop over all pixels
    for (size_t y = 1; y < height - 1; ++y) {
        for (size_t x = 1; x < width - 1; ++x) {
            double value = 0.0;

            // Convolve kernel with pixel neighborhood
            for (int j = -1; j <= 1; ++j) {
                for (int i = -1; i <= 1; ++i) {
                    size_t index = (y + j) * width + (x + i);
                    value += kernel[j + 1][i + 1] * (is_gx ? pixels[index + 1].red : pixels[index].red);
                }
            }

            // Normalize the value and store it in output image
            value = std::min(std::max(value, 0.0), 65535.0);
            output.pixelColor(x, y, Magick::ColorGray(value));
        }
    }

    return output;
}

