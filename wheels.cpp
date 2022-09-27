// wheels.cpp

#include "wheels.h"
#include <VarSpeedServo.h>
#include <myDebug.h>
#include "compass.h"

#define DEBUG_WHEELS 	  0

// Weirdness ...
// For some reason the servo on wheel one has the opposite polarity of
// wheel four (all other wheels) ....



#define WHEEL_LEFT_FRONT  0
#define WHEEL_LEFT_BACK   1
#define WHEEL_RIGHT_BACK  2
#define WHEEL_RIGHT_FRONT 3

#define PIN_WHEEL(w)	(PIN_WHEEL_FL + (w))


uint16_t 	wheels::m_heading;
uint8_t 	wheels::m_duration;
uint32_t 	wheels::m_start_time;
uint32_t 	wheels::m_home_time;


VarSpeedServo wheel_servo[4];


void wheels::init()
{
	for (int i=0; i<4; i++)
	{
		wheel_servo[i].attach(PIN_WHEEL_FL + i);
		wheel_servo[i].write(90);
		delay(100);
		wheel_servo[i].detach();
	}
}


void wheels::setHeading()
{
	#if WITH_COMPASS
		m_heading = read_compass_heading();
	#endif
}




void wheels::update()
{
	uint32_t now = millis();

	#if WITH_COMPASS
		if (m_home_time && now > m_home_time)	// every millisecond
		{
			home(false);
			#if DEBUG_WHEELS
				if (!m_home_time) display(0,"home stopped %d",read_compass_heading());
			#endif
			return;	// homing doesnt honor durations ...
		}
	#endif

	if (m_start_time &&
		m_duration != INFINITE_DURATION &&
		now > m_start_time + m_duration * 10)
	{
		#if DEBUG_WHEELS
			display(0,"move end",0);
		#endif
		stop();
	}
}

bool wheels::busy()
{
	return m_home_time || attached();
}

void wheels::stop()
{
	m_start_time = 0;
	for (int i=0; i<4; i++)
	{
		wheel_servo[i].write(90);
		wheel_servo[i].stop();
		wheel_servo[i].detach();
	}
	delay(5);
}


bool wheels::attached()
{
	for (int i=0; i<4; i++)
	{
		if (wheel_servo[i].attached())
			return true;
	}
	return false;
}
bool wheels::moving()
{
	for (int i=0; i<4; i++)
	{
		if (wheel_servo[i].isMoving())
			return true;
	}
	return false;
}
int  wheels::read(uint8_t num)
{
	return wheel_servo[num].read();
}


void wheels::move(const int *dir, uint8_t duration, uint8_t speed, uint8_t rate)
{
	m_duration = duration;

	#if DEBUG_WHEELS
		display(0,"move(%d,%d,%d)",duration,speed,rate);
	#endif

	for (int i=0; i<4; i++)
	{
		int value = 90 + speed * dir[i];
		wheel_servo[i].attach(PIN_WHEEL_FL + i);
		wheel_servo[i].write(value,rate);
	}
	m_start_time = millis();
}


// for some reason wheel 1 and wheel4 have opposite polarities

void wheels::left(uint8_t duration, uint8_t speed, uint8_t rate)
{
	const int dir[] = {-1,-1,-1,1};
	move(dir,duration,speed,rate);
}

void wheels::right(uint8_t duration, uint8_t speed, uint8_t rate)
{
	const int dir[] = {1,1,1,-1};
	move(dir,duration,speed,rate);
}
void wheels::cw(uint8_t duration, uint8_t speed, uint8_t rate)
{
	const int dir[] = {-1,1,1,1};
	move(dir,duration,speed,rate);
}
void wheels::ccw(uint8_t duration, uint8_t speed, uint8_t rate)
{
	const int dir[] = {1,-1,-1,-1};
	move(dir,duration,speed,rate);
}



void wheels::home(bool cold)
{
	#if WITH_COMPASS

		int dir = 0;
		int speed = 0;

		static int last_dir = 0;
		static int last_speed = 0;

		if (cold)
		{
			last_dir = 0;
			last_speed = 0;
			m_home_time = 0;
		}

		int heading = read_compass_heading();		// 0..359
		int diff = heading - m_heading;
		if (diff >  180) diff -= 360;
		if (diff < -180) diff += 360;
		if (diff < -2)
		{
			speed = diff<-20?30:10;
			dir = -1;
		}
		else if (diff > 2)
		{
			speed = diff>20?30:10;
			dir = 1;
		}

		#if DEBUG_WHEELS
			display(0,"home(%d) %d %d %d  speed=%d  dir=%d",cold,heading,m_heading,diff,speed,dir);
		#endif

		if (last_speed != speed ||
			last_dir != dir)
		{
			last_speed = speed;
			last_dir = dir;
			if (dir == -1)
			{
				ccw(255, speed, 0);
				m_home_time = millis();
			}
			else if (dir == 1)
			{
				cw(255, speed, 0);
				m_home_time = millis();
			}
			else
			{
				stop();
				m_home_time = 0;
			}
		}

	#endif
}
