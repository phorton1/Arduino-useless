// arm.cpp

#include "lid.h"
#include <VarSpeedServo.h>

#if USELESS_VERSION == 1
	#define LID_PEEK_DEGREES  17
	#define LID_SMALL_DEGREES 26
	#define LID_OPEN_DEGREES  40
#else
	#define LID_PEEK_DEGREES  22
	#define LID_SMALL_DEGREES 35
	#define LID_OPEN_DEGREES  45
#endif


VarSpeedServo lid_servo;


void lid::init()
{
	lid_servo.attach(PIN_LID);
	lid_servo.write(0);
	delay(500);
	lid_servo.detach();
}


bool lid::busy()
{
	if (lid_servo.attached() && !lid_servo.isMoving())
	{
		// servo tuning - i had to put a delay here or else the servo did
		// not get to the desired position for the flapper ... lowering the
		// speed did not seem to help ?!?!? it is as if the servo
		// moves slower than the library !?!?

		#if USELESS_VERSION == 1
			delay(20);
		#else
			delay(50);
		#endif
		lid_servo.detach();
	}
	return lid_servo.attached();
}

void lid::stop()
{
	lid_servo.stop();
	lid_servo.detach();
}


// pass thrus for debuggin
bool lid::attached()
{
	return lid_servo.attached();
}
bool lid::moving()
{
	return lid_servo.isMoving();
}
int lid::read()
{
	return lid_servo.read();
}



void lid::close(uint8_t rate)
{
	lid_servo.attach(PIN_LID);
	lid_servo.write(0,rate,false);	// don't wait
}

void lid::peek(uint8_t rate)
{
	lid_servo.attach(PIN_LID);
	lid_servo.write(LID_PEEK_DEGREES,rate,false);	// don't wait
}

void lid::small(uint8_t rate)
{
	lid_servo.attach(PIN_LID);
	lid_servo.write(LID_SMALL_DEGREES,rate,false);	// don't wait
}


void lid::open(uint8_t rate)
{
	lid_servo.attach(PIN_LID);
	lid_servo.write(LID_OPEN_DEGREES,rate,false);	// don't wait
}
