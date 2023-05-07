#pragma once
#ifndef STRONKIMAGE_PIXEL
#define STRONKIMAGE_PIXEL

#include <stdint.h>

#define QuantumRange 255

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

		// Overload the == operator for RGBPixelBuf
		bool operator==(const _RGBPixelBuf& other) const
		{
			return red == other.red
				&& green == other.green
				&& blue == other.blue
				&& opacity == other.opacity;
		}
	} RGBPixelBuf;
}

#endif