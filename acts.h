// acts.h

#pragma once

#include "useless.h"


// I understand basic synchronous steps, where one happens after the previous one is complete.
// Pixels can be changed with no jitter in-between steps, i.e. there are probably "pixel" steps,
// so I can envision, for instance, flash-while-flapping using a loop control constuct.
//
// There are several other things I would like to do:
//
//      - flap while (smooth) rotating
//      - going to differnent states based on IR values
//
//I am having a hard time getting my head around an asynchronous language that can do things like flap while rotating
// while having pixels flash, orchestrate different levels based on IR sensing, and so on.



//----------------------------------------------------------
// "Acts" and "Steps"
//----------------------------------------------------------
// Acts are made of steps.
//
// Steps can be of several basic types:
//  	- Servo Moves with positions and rates
//      - Wheel Moves with ratesdurations
//      - Pixel Changes
//      - Control Steps


// By default, the next "act" "starts" when the switch is turned on.
//
// We start with simple steps that are atomic servo motor movements.
// Each can be given a fixed delay before it begins and proceeds at a fixed rate.
// I envision the delays and rates being randomized over repeated instances of the acts

#define STEP_TYPE_MOVE  	0x01
#define STEP_TYPE_PIXELS	0x02
#define STEP_TYPE_LOOP		0x03

#define LID_CLOSED		0x0001
#define LID_PEEK		0x0002
#define LID_SMALL		0x0004
#define LID_OPEN		0x0008

#define ARM_DOWN		0x0010
#define ARM_UP			0x0020
#define ARM_OUT			0x0040
#define ARM_POISED		0x0080

#define ARM_TURN_OFF_POSITION	0x0100
	// goes to 91 degrees, *may* turn off switch
	// used as the "lock" position in IR sycle
#define ARM_TURN_OFF			0x0200
	// TRANSITION TO SWTICH OFF STATE
	// invariant moves to 92+ degrees or until the switch goes off
#define WAIT_NO_IR				0x0400
	// waits for no ir then does the step


#define WHEELS_LEFT		0x1000
#define WHEELS_RIGHT	0x2000
#define WHEELS_CW		0x4000
#define WHEELS_CCW		0x8000
	// these are given a duration in milliseconds
	// where 150 is a burst and 2 seconds a rotation
#define WHEELS_HOME		0x0800
	// return to heading from the start of the act


void start_act();
	// the next act is started when the switch transitions to the on state
	// interrupting any act that is currently in progress

void process_act();
	// called at frame rate - implements all behavior

void set_move_mode(int mode);
	// 0 = reset the interpreter (start new session)
	// 1 = go to MOVE_MODE_1
	// 2 = go to MOVE_MODE_2
