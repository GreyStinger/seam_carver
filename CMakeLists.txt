# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(seamcarver)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)

# Find the ImageMagick, libpng, and libjpeg libraries
find_package(ImageMagick COMPONENTS Magick++ REQUIRED)
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)

# Set the include directories
include_directories(
    ${ImageMagick_INCLUDE_DIRS}
    ${PNG_INCLUDE_DIRS}
    ${JPEG_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

# Set the source files
file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Compile the source files to object files
add_library(objects OBJECT ${SRC_FILES})

# Set the executable name and link the object files and the libraries
add_executable(seamcarve $<TARGET_OBJECTS:objects>)
target_link_libraries(seamcarve ${ImageMagick_LIBRARIES} ${PNG_LIBRARIES} ${JPEG_LIBRARIES})
