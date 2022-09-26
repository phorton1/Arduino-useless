// useless.h

#pragma once

#include <Arduino.h>


#define PIN_ARM				2
#define PIN_LID		  		3
#define PIN_WHEEL_FL  		4
#define PIN_WHEEL_BL		5
#define PIN_WHEEL_BR		6
#define PIN_WHEEL_FR		7
#define PIN_UNUSED_SERVO	8
#define PIN_SWITCH			9
#define PIN_LED_STRIP		10
#define PIN_BUTTON1			11
#define PIN_BUTTON2			12
#define PIN_L_LED			A1	// jumper wire
#define PIN_L_SENSE			A6
#define PIN_SCL				A5
#define PIN_SDA				A4
#define PIN_R_LED			A3
#define PIN_R_SENSE			A2
#define PIN_UNUSED_LED		A0	// jumper_wire - mistake currently wired to V_REF
#define PIN_UNUSED_SENSE    13

#define SERVO_ARM			0
#define SERVO_LID		  	1
#define SERVO_WHEEL_FL  	2
#define SERVO_WHEEL_BL		3
#define SERVO_WHEEL_BR		4
#define SERVO_WHEEL_FR		5
#define NUM_SERVOS   		6

#define WITH_PIXELS			1
#define WITH_IR				1
#define WITH_COMPASS		1

extern bool switch_state;
