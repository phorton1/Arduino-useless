// lid.h

#pragma once

#include "useless.h"




class lid
{
public:

	static void init();
	static bool busy();
	static void stop();

	static bool attached();
	static bool moving();
	static int read();

	static void close(uint8_t rate=255);
	static void peek(uint8_t rate=255);
	static void small(uint8_t rate=255);
	static void open(uint8_t rate=255);
};
