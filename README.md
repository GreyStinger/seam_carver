# Single-Threaded C++ Seam Carving Tool

This is a single-threaded C++ implementation of the seam carving algorithm. Seam carving is an image processing technique that can be used to resize images while preserving important content in the image. This implementation uses dynamic programming to find the seams with the lowest energy in the image and removes them to resize the image.

## Requirements

The following dependencies are required to build and run the tool:

- `pkg-config`
- `libpng`
- `libjpeg`

You can confirm whether they are installed by running the configure script. You will see in in the next step

## Building the Tool

To build the tool, follow these steps:

1. Clone the repository to your local machine:

    ```bash
    git clone https://github.com/GreyStinger/seamcarver.git
    ```

2. Create a subdirectory called `build` and generate build flags with CMake:

    The intended way:

    ```bash
    ./configure
    ```

    Otherwise do it manually:

    ```bash
    cd seamcarver
    mkdir build
    cd build

    cmake ..
    ```

3. Build the tool using make:

    ```bash
    make
    ```

    This will create an executable called `seamcarver` in the `build` directory.

4. Optionally you can install the tool to your bin directory with:

   ```bash
   sudo make install
   ```

## Running the Tool

To run the tool, use the following command:

```bash
./seamcarver <input-image> <output-image> <num-seams>
```

- `<input-image>` is the path to the input image file.
- `<output-image>` is the path to the output image file.
- `<num-seams>` is the number of seams to remove from the image.

For example:

```bash
./seamcarver input.jpg output.jpg 100
```

This will resize the image `input.jpg` by removing 100 seams and save the result to `output.jpg`.

## Limitations

This implementation of the seam carving algorithm is single-threaded, which means that it may be slow for large images or when removing a large number of seams. If you need to process images quickly or in parallel, you may want to consider using a multi-threaded implementation or a GPU-accelerated implementation of the algorithm.

## Credits

This implementation of the seam carving algorithm is based on the original paper by Shai Avidan and Ariel Shamir - though it only does vertical seamcarving so far -, "Seam Carving for Content-Aware Image Resizing", which can be found here: <https://perso.crans.org/frenoy/matlab2012/seamcarving.pdf>.
