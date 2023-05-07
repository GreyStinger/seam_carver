#pragma once
#ifndef STRONKIMAGE_PIXEL
#define STRONKIMAGE_PIXEL

#include <stdint.h>

namespace StronkImage
{
	// Define Quantum for readability
	typedef uint32_t Quantum;

	// Define colour pixel data
	typedef struct _RGBPixelBuf
	{
		Quantum 
			red, 
			green, 
			blue, 
			opacity;
	} RGBPixelBuf;
}

#endif