// pixels.cpp

#include "pixels.h"

#if WITH_PIXELS

	Adafruit_NeoPixel pixels(NUM_PIXELS, PIN_LED_STRIP, NEO_GRB + NEO_KHZ800);

	void init_pixels()
	{
		pixels.begin();
		pixels.setBrightness(120);
		pixels.clear();

		pixels.show();
		for (int j=0; j<3; j++)
		{
			for (int i=0; i<NUM_PIXELS; i++)
			{
				delay(50);
				pixels.setPixelColor(i,
					j==0?255:0,
					j==1?255:0,
					j==2?255:0);
				pixels.show();
			}
			delay(100);
		}
		// leaves row of blue showing
		}


	void setLeftPixels(uint8_t r, uint8_t g, uint8_t b, bool show /*=true*/)
	{
		for (int i=0; i<3; i++)
		{
			pixels.setPixelColor(i+PIXEL_LEFT,r,g,b);
		}
		if (show)
			pixels.show();
	}

	void setRightPixels(uint8_t r, uint8_t g, uint8_t b, bool show /*=true*/)
	{
		for (int i=0; i<3; i++)
		{
			pixels.setPixelColor(i+PIXEL_RIGHT,r,g,b);
		}
		if (show)
			pixels.show();
	}
#endif
