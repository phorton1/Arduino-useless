// arm.cpp

#include "arm.h"
#include <VarSpeedServo.h>
#include <myDebug.h>

#define DEBUG_ARM    0

#define ARM_UP_DEGREES 50
#define ARM_OUT_DEGREES 80
#define ARM_POISED_DEGREES 98
#define ARM_OFF_DEGREES 110
#define ARM_TURN_OFF_DEGREES 115


bool arm::m_wait_off;


VarSpeedServo arm_servo;


void arm::init()
{
	arm_servo.attach(PIN_ARM);
	arm_servo.write(0);
	delay(100);
	arm_servo.detach();
}


bool arm::busy()
{
	if (m_wait_off)
	{
		if (!switch_state)
		{
			m_wait_off = false;
		}
	}
	else if (arm_servo.attached() && !arm_servo.isMoving())
	{
		arm_servo.detach();
	}
	return arm_servo.attached() || m_wait_off;
}


// pass thrus for debuggin
bool arm::attached()
{
	return arm_servo.attached();
}
bool arm::moving()
{
	return arm_servo.isMoving();
}
int arm::read()
{
	return arm_servo.read();
}


void arm::stop()
{
	m_wait_off = 0;
	arm_servo.stop();
	arm_servo.detach();
}

void arm::down(uint8_t rate)
{
	arm_servo.attach(PIN_ARM);
	arm_servo.write(0,rate,false);	// don't wait
	#if DEBUG_ARM
		display(0,"arm_down(%d)",0);
	#endif
}

void arm::up(uint8_t rate)
{
	arm_servo.attach(PIN_ARM);
	arm_servo.write(ARM_UP_DEGREES,rate,false);	// don't wait
	#if DEBUG_ARM
		display(0,"arm_up(%d)",ARM_UP_DEGREES);
	#endif
}

void arm::out(uint8_t rate)
{
	arm_servo.attach(PIN_ARM);
	arm_servo.write(ARM_OUT_DEGREES,rate,false);	// don't wait
	#if DEBUG_ARM
		display(0,"arm_out(%d)",ARM_OUT_DEGREES);
	#endif
}

void arm::poised(uint8_t rate)
{
	arm_servo.attach(PIN_ARM);
	arm_servo.write(ARM_POISED_DEGREES,rate,false);	// don't wait
	#if DEBUG_ARM
		display(0,"arm_poised(%d)",ARM_POISED_DEGREES);
	#endif
}

void arm::off_position(uint8_t rate)
{
	arm_servo.attach(PIN_ARM);
	arm_servo.write(ARM_OFF_DEGREES,rate,false);	// don't wait
	#if DEBUG_ARM
		display(0,"arm_off(%d)",ARM_OFF_DEGREES);
	#endif
}

void arm::turn_off_switch(uint8_t rate)
{
	if (switch_state)
	{
		arm_servo.attach(PIN_ARM);
		arm_servo.write(ARM_TURN_OFF_DEGREES,rate,false);	// don't wait
		#if DEBUG_ARM
			display(0,"arm_turn_off(%d)",ARM_TURN_OFF_DEGREES);
		#endif
		m_wait_off = true;
	}
}
