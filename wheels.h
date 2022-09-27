// wheels.h

#pragma once

#include "useless.h"


class wheels
{
public:

	static void init();
	static void setHeading();
	static void update();
		// to be called directly from loop,
		// every millisecond checks if wheels should be stopped


	static bool busy();
	static void stop();

	static bool attached();
	static bool moving();
	static int read(uint8_t num);

	// durations are in 100's of a second
	// speeds are 0..90 !!

	static void left(uint8_t duration, uint8_t speed=90, uint8_t rate=0);
	static void right(uint8_t duration, uint8_t speed=90, uint8_t rate=0);
	static void cw(uint8_t duration, uint8_t speed=90, uint8_t rate=0);
	static void ccw(uint8_t duration, uint8_t speed=90, uint8_t rate=0);
	static void home(bool cold);

private:

	static uint16_t m_heading;
	static uint8_t m_duration;
	static uint32_t m_start_time;
	static uint32_t m_home_time;

	static void move(const int *dir, uint8_t duration, uint8_t speed, uint8_t rate);

};