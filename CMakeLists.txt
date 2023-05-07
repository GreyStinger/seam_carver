# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(seamcarver)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)

# Set the build type to Debug to include debugging symbols
set(CMAKE_BUILD_TYPE Debug)

# Find the libpng, libjpeg, and Google Test libraries
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
find_package(GTest REQUIRED)

# Set the include directories
include_directories(
    ${PNG_INCLUDE_DIRS}
    ${JPEG_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${GTEST_INCLUDE_DIRS}
)

# Set the source files for the libraries
file(GLOB_RECURSE LIBS_SRC_FILES ${CMAKE_SOURCE_DIR}/libs/*.cpp)
file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Compile the source files to object files
add_library(libs_objects OBJECT ${LIBS_SRC_FILES})
add_library(src_objects OBJECT ${SRC_FILES})

# Set the executable name and link the object files and the libraries
add_executable(seamcarve $<TARGET_OBJECTS:src_objects>)
target_link_libraries(seamcarve ${PNG_LIBRARIES} ${JPEG_LIBRARIES} src_objects libs_objects)

# Link the libraries with the object files from the src folder
target_link_libraries(src_objects libs_objects)

# Add your test source files
set(TEST_SOURCES
    tests/ImageDataTest.cpp
    # Add more test files if needed
)

# Create the test executable
add_executable(seamcarver_tests ${TEST_SOURCES})

# Link the Google Test libraries and your project's object files to your test executable
target_link_libraries(seamcarver_tests GTest::GTest GTest::Main ${PNG_LIBRARIES} ${JPEG_LIBRARIES} libs_objects)

# Enable testing and add your test executable as a test
enable_testing()
add_test(NAME seamcarver_tests COMMAND seamcarver_tests)

# Installation settings
install(TARGETS seamcarve DESTINATION bin)

# Add uninstall target
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

