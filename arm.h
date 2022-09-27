// arm.h

#pragma once

#include "useless.h"

class arm
{
public:

	static void init();
	static bool busy();
	static void stop();

	static bool attached();
	static bool moving();
	static int read();


	static void down(uint8_t rate=255);
	static void up(uint8_t rate=255);
	static void out(uint8_t rate=255);
	static void poised(uint8_t rate=255);
	static void off_position(uint8_t rate=255);
	static void turn_off_switch(uint8_t rate=255);
	static void turn_off_no_ir(uint8_t rate=255);

private:

	static bool m_wait_off;


};
