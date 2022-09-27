// arm.cpp

#include "lid.h"
#include <VarSpeedServo.h>

#define LID_PEEK_DEGREES  20
#define LID_SMALL_DEGREES 30
#define LID_OPEN_DEGREES  47



VarSpeedServo lid_servo;


void lid::init()
{
	lid_servo.attach(PIN_LID);
	lid_servo.write(0);
	delay(100);
	lid_servo.detach();
}


bool lid::busy()
{
	if (lid_servo.attached() && !lid_servo.isMoving())
	{
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
