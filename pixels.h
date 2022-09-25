// pixels.h

#include "useless.h"

#if WITH_PIXELS
	#include "Adafruit_NeoPixel.h"

	#define NUM_PIXELS   11

	#define PIXEL_RIGHT		0
	#define PIXEL_LEFT		3
	#define PIXEL_USER		6

	extern Adafruit_NeoPixel pixels;

	extern void init_pixels();
	extern void setLeftPixels(uint8_t r, uint8_t g, uint8_t b, bool show=true);
	extern void setRightPixels(uint8_t r, uint8_t g, uint8_t b, bool show=true);
#endif