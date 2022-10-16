// acts.cpp

#include "acts.h"
#include "arm.h"
#include "lid.h"
#include "wheels.h"
#include "ir.h"
#include "pixels.h"
#include "myDebug.h"

// The "program" proceeds through passes, based on the number of defined acts,
// increasing in random and more agressive behavior as it goes through the passes.
//
// Movements fall into three categories.  Small moves that can happen on a cluttered shelf
// are included by default and is called MOVE_MODE_0. Slightly bigger movements require button
// presses to set MOVE_MODE1, which includes upto ONE evasive step and other specific steps.
// Full circles, more than one evasive step require higher MOVE_MODE_2.


// Each step can be assigned a "minimum pass" that it takes effect in, so
// for instance, we could have an act that has no wheel moves in PASS0 and PASS1, and
// then starts having wheel moves in PASS2.
//
// When an act is not in progress is called an "in between behavior" which includes
// the ir_arm_mode (where the arm pops out based on an IR signal) and evasion_mode
// where the box moves to avoid the user.  ir_arm_mode begins in pass2, and evasion_mode
// only begins at AGRESSION level 1 AFTER at least two other moves (jitters) have
// happened (to prevent giving away the fact that the box can move)l
//
// Furthermore we can set an agression level (via a button), that defaults to zero,
// and steps can also be limited to only operate at a given agression level.  For
// instance, spinning the box requires a lot of space, and may not be appropriate
// when the box is sitting on a cluttered shelf.  So we can say that apinning the
// box only happens at AGRESSION 2.
//
// In addition each step has several items that can be individually randomized:
//
//     - delay before the step
//     - servo rates
//     - number of repeats
//     - (wheel duration)
//
// The randomization only begins occuring on the second pass.
//
//		PASS1 - acts are performed in defined order, using median values
//              for random variables (min+max/2).  No movements are performed.
//      PASS2 - adds IR mode and randomization of parameters
//      PASS3 - adds 1st level of moves (jitters)
//
//      after at least two 1st level moves have happened, evasion mode
//      starts taking place if AG, and if AGRESSION is set to a certain
//
//

#define DEBUG_ACT  0


#define TIMEOUT_SEESIONS   0
	// if set to 1 the sessions will timeout.
	// if 0, session can still be restarted via call (button)

#define SESSION_TIMEOUT (2UL*60UL*1000UL)
    // session times out after two minutes
    //
    // NOTE COMPILER ISSUE:
    //
    // I was getting weird behavior when I used (2*60*1000) .. without any compiler warnings.
    // I switched to a "new bootloader" nano and started getting a warning about an integer
    // overflow when using the above define.  It turns out that by default all constants on the
    // right side of an expression are automatically 16 bits on Arduino and you MUST use the UL
    // designation to get around it.

#pragma pack 1

// delays and durations are in 100's of a second

typedef struct
{
	uint8_t	step_type;
	uint8_t min_pass;
	uint8_t min_mode;

	union {
		struct {
			uint16_t 	move_bits;
			uint8_t     delay1;
			uint8_t     delay_min;
			uint8_t     delay_max;

			uint8_t     rate;
			uint8_t		wheel_duration;
		} move;
		struct
		{
			uint8_t		lr;
			uint8_t		lg;
			uint8_t		lb;
			uint8_t		rr;
			uint8_t		rg;
			uint8_t		rb;
		} pixels;
		struct
		{
			uint8_t		num_steps;
			uint8_t		repeat1;
			uint8_t		repeat_min;
			uint8_t		repeat_max;
		} loop;
	};

}	step_t;


typedef step_t *act_t;		// an act is an array of steps

#define BLACK   0,0,0
#define RED  	255,0,0
#define GREEN   0,255,0
#define BLUE    0,0,255
#define CYAN    0,255,255
#define MAGENTA 255,0,255
#define YELLOW  255,255,0
#define WHITE   255,255,255



const step_t normal_medium[] PROGMEM = {
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{ARM_DOWN, 						0,0,0,	 		255,   0}}},
	{ STEP_TYPE_RANDOM },
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{LID_OPEN, 						0,0,0,	 		 50,   0}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{ARM_TURN_OFF,					0,0,0,	 		 50,   0}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{ARM_DOWN, 						0,0,0,	 		 50,   0}}},
	{ STEP_TYPE_RANDOM },
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{LID_CLOSED, 					0,0,0,	 		 50,   0}}},
	{ 0 }
};


const step_t quick_poised[] PROGMEM = {
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{ARM_DOWN, 						0,0,0, 			255,   0}}},
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{BLUE, BLUE}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{LID_OPEN, 		  				0,0,0,			255,   0}}},
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{MAGENTA, MAGENTA}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{ARM_POISED, 		 			60,50,120, 		255,   0}}},
	{ STEP_TYPE_RANDOM },
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{ARM_TURN_OFF,					120,100,150,	100,   0}}},
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{GREEN, GREEN}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{ARM_POISED,					0,0,0, 		 	 20,   0}}},
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{CYAN, CYAN}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{ARM_DOWN, 						100,100,150, 	255,   0}}},
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{BLACK, BLACK}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{LID_CLOSED, 			    	0,0,0,  		 50,   0}}},
	{ 0 }
};



const step_t fastest[] PROGMEM = {
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{GREEN, GREEN}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{ARM_TURN_OFF,					0,0,0,			255,   0}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{ARM_DOWN | LID_CLOSED, 		0,0,0,			255,   0}}},
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{BLACK, BLACK}}},
	{ 0 }
};

const step_t quick_small_delayed[] PROGMEM = {
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{RED, RED}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{LID_SMALL,						100,20,100, 	 30,   0}}},
	{ STEP_TYPE_RANDOM },
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{ARM_TURN_OFF,					150,50,150, 	255,   0}}},
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{BLUE, BLUE}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{ARM_DOWN, 						0,0,0,			255,   0}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{LID_CLOSED, 					100,0,100,  	 40,    0}}},
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{BLACK, BLACK}}},
	{ 0 }
};

const step_t fastest_no_ir[] PROGMEM = {
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{RED, RED}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{ARM_TURN_OFF | WAIT_NO_IR,		0,0,0,			255,   0}}},
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{GREEN, GREEN}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{ARM_DOWN | LID_CLOSED, 		0,0,0,			255,   0}}},
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{BLACK, BLACK}}},
	{ 0 }
};



const step_t arm_repeat[] PROGMEM = {
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{MAGENTA, MAGENTA}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{LID_OPEN, 						100,0,100, 		80,   0}}},
	{ STEP_TYPE_RANDOM },
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{RED, RED}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{ARM_UP, 					  	0,0,0,			255,   0}}},
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{MAGENTA, MAGENTA}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{ARM_POISED,				    0,0,0,			255,   0}}},
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{RED, RED}}},
	{ STEP_TYPE_LOOP,   0,0,	{.loop=		{5,4,2,6}}},
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{GREEN, GREEN}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{ARM_TURN_OFF,					100,50,150,		255,   0}}},
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{BLUE, BLUE}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{ARM_DOWN, 						0,0,0,			255,   0}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{LID_CLOSED, 					0,0,0,  		 50,   0}}},
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{BLACK, BLACK}}},
	{ 0 }
};


// flapper explicitly moves ARM_DOWN and LID_CLOSED to begin in case of ir_mode interrupt
// see flapper specific servo tuning in lid.cpp ?!?

const step_t flapper[] PROGMEM = {
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{ARM_DOWN | LID_CLOSED,			  0,0,0,		255,   0}}},
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{GREEN, RED}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{LID_SMALL, 					  0,0,0,		192,   0}}},
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{GREEN, RED}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{LID_CLOSED, 					  0,0,0,		255,   0}}},
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{RED, GREEN}}},
	{ STEP_TYPE_LOOP,   0,0,	{.loop=		{5,3,0,4}}},
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{BLUE, BLUE}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{LID_SMALL,						  0,0,0,		192,   0}}},
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{CYAN, CYAN}}},
	{ STEP_TYPE_RANDOM },
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{ARM_TURN_OFF,					  0,0,0,		255,   0}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{ARM_DOWN | LID_CLOSED, 		  0,0,0,		255,   0}}},
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{GREEN, RED}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{LID_SMALL, 					  0,0,0,		192,   0}}},
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{GREEN, RED}}},
	{ STEP_TYPE_MOVE,	0,0,	{.move=		{LID_CLOSED, 					  0,0,0,		255,   0}}},
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{RED, GREEN}}},
	{ STEP_TYPE_LOOP,   0,0,	{.loop=		{0,0,0,4}}},
	{ STEP_TYPE_PIXELS, 0,0,	{.pixels=	{BLACK, BLACK}}},
	{ 0 }
};



//----------------------------
// random moves
//----------------------------
// starting with pass2, these will be inserted randomly at the places in the main acts
// each entire act must be in a specific pass, as we use the first step's pass value to skip
// the whole sequence. for the time being they should not be in loops as we do not preserve
// the loop counter.


#define WHEEL_TIME  3
#define ROTATE_TIME 230

#define MP2 2
#define MP3 3

const step_t random_rotate_cw[] PROGMEM = {
	{ STEP_TYPE_PIXELS, MP2,1,	{.pixels=	{YELLOW, YELLOW}}},
	{ STEP_TYPE_MOVE,	MP2,1,	{.move=		{WHEELS_CW, 		  			0,0,0,	 	  	 0,    ROTATE_TIME}}},
	{ STEP_TYPE_PIXELS, MP2,1,	{.pixels=	{GREEN, GREEN}}},
	{ STEP_TYPE_MOVE,	MP2,2,	{.move=		{WHEELS_HOME,					0,0,0,	 	  	 0,    0}}},
	{0},
};

const step_t random_rotate_ccw[] PROGMEM = {
	{ STEP_TYPE_PIXELS, MP2,1,	{.pixels=	{RED, RED}}},
	{ STEP_TYPE_MOVE,	MP2,1,	{.move=		{WHEELS_CCW, 		  			0,0,0,	 	  	 0,    ROTATE_TIME}}},
	{ STEP_TYPE_PIXELS, MP2,1,	{.pixels=	{GREEN, GREEN}}},
	{ STEP_TYPE_MOVE,	MP2,2,	{.move=		{WHEELS_HOME,					0,0,0,	 	  	 0,    0}}},
	{0},
};

const step_t back_and_forth[] PROGMEM = {
	{ STEP_TYPE_PIXELS, MP2,1,	{.pixels=	{MAGENTA, MAGENTA}}},
	{ STEP_TYPE_MOVE,	MP2,1,	{.move=		{WHEELS_CW, 		  			0,0,0,	 	  	 0,    40}}},
	{ STEP_TYPE_PIXELS, MP2,1,	{.pixels=	{RED, RED}}},
	{ STEP_TYPE_MOVE,	MP2,1,	{.move=		{WHEELS_CCW, 		  			0,0,0,	 	  	 0,    80}}},
	{ STEP_TYPE_PIXELS, MP2,1,	{.pixels=	{BLUE, BLUE}}},
	{ STEP_TYPE_MOVE,	MP2,1,	{.move=		{WHEELS_CW, 		  			0,0,0,	 	  	 0,    39}}},
	{ STEP_TYPE_PIXELS, MP2,1,	{.pixels=	{GREEN, GREEN}}},
	{ STEP_TYPE_MOVE,	MP2,2,	{.move=		{WHEELS_HOME,					0,0,0,	 	  	 0,    0}}},
	{0},
};

const step_t random_jiggle[] PROGMEM = {
	{ STEP_TYPE_PIXELS, MP2,0,	{.pixels=	{MAGENTA, MAGENTA}}},
	{ STEP_TYPE_MOVE,	MP2,0,	{.move=		{WHEELS_CCW, 		  			0,0,0,	 		  0,   WHEEL_TIME}}},
	{ STEP_TYPE_PIXELS, MP2,0,	{.pixels=	{YELLOW,YELLOW}}},
	{ STEP_TYPE_MOVE,	MP2,0,	{.move=		{WHEELS_CW,						0,0,0,	 		  0,   WHEEL_TIME}}},
	{ STEP_TYPE_PIXELS, MP2,0,	{.pixels=	{MAGENTA, MAGENTA}}},
    { STEP_TYPE_LOOP,	MP2,0,	{.loop=		{5,2,3,7}}},
	{ STEP_TYPE_MOVE,	MP2,2,	{.move=		{WHEELS_HOME,					0,0,0,	 	  	 0,    0}}},
	{0},
};

const step_t random_left_right[] PROGMEM = {
	{ STEP_TYPE_PIXELS, MP2,0,	{.pixels=	{MAGENTA, MAGENTA}}},
	{ STEP_TYPE_MOVE,	MP2,0,	{.move=		{WHEELS_LEFT, 		  			0,0,0,	 		  0,   WHEEL_TIME}}},
	{ STEP_TYPE_PIXELS, MP2,0,	{.pixels=	{BLUE,BLUE}}},
	{ STEP_TYPE_MOVE,	MP2,0,	{.move=		{WHEELS_RIGHT,					0,0,0,	 		  0,   WHEEL_TIME+1}}},
	{ STEP_TYPE_PIXELS, MP2,0,	{.pixels=	{MAGENTA, MAGENTA}}},
    { STEP_TYPE_LOOP,	MP2,0,	{.loop=		{5,2,3,7}}},
	{ STEP_TYPE_MOVE,	MP2,2,	{.move=		{WHEELS_HOME,					0,0,0,	 	  	 0,    0}}},
	{0},
};


const step_t random_right_left[] PROGMEM = {
	{ STEP_TYPE_PIXELS, MP2,0,	{.pixels=	{MAGENTA, MAGENTA}}},
	{ STEP_TYPE_MOVE,	MP2,0,	{.move=		{WHEELS_RIGHT, 		  			0,0,0,	 		  0,   WHEEL_TIME}}},
	{ STEP_TYPE_PIXELS, MP2,0,	{.pixels=	{CYAN,CYAN}}},
	{ STEP_TYPE_MOVE,	MP2,0,	{.move=		{WHEELS_LEFT,					0,0,0,	 		  0,   WHEEL_TIME}}},
	{ STEP_TYPE_PIXELS, MP2,0,	{.pixels=	{MAGENTA, MAGENTA}}},
    { STEP_TYPE_LOOP,	MP2,0,	{.loop=		{5,2,3,7}}},
	{ STEP_TYPE_MOVE,	MP2,2,	{.move=		{WHEELS_HOME,					0,0,0,	 	  	 0,    0}}},
	{0},
};



//--------------------------------------
// list of acts
//--------------------------------------
// the frequency of an act in a table determines it's
// relative probability of being triggered

act_t acts[] = {
	normal_medium,
	quick_poised,
	fastest,
	fastest_no_ir,
	quick_small_delayed,
	fastest_no_ir,
	arm_repeat,
	fastest_no_ir,
	flapper,
};


#define NUM_ACTS	(sizeof(acts)/sizeof(act_t *))


act_t random_acts[] = {
	random_rotate_cw,
	random_rotate_ccw,
	back_and_forth,
	random_jiggle,
	random_jiggle,
	random_left_right,
	random_left_right,
	random_right_left,
	random_right_left,
};


#define NUM_RANDOM_ACTS	(sizeof(random_acts)/sizeof(act_t *))


//--------------------------------------
// global variables
//--------------------------------------

int move_mode = 1;
	// normally 0, 1 for first cantina demo!
uint32_t session_start;
int session_act_count;
int session_pass;

bool in_act;
int cur_act_num = -1;

act_t cur_act;
act_t save_act;
int cur_step_num;
int save_step_num;
int act_moved;

int loop_count;
bool ir_mode = 0;
bool obs_mode = 0;
int obs_pos = 0;


void handle_ir();	// forward
void handle_obs();	// forward

// progmem accessor

step_t ram_step;
step_t *getStep(act_t act, int step_num)
{
	memcpy_P(&ram_step,&act[step_num],sizeof(step_t));
	return &ram_step;
}


// session control

void set_move_mode(int mode)
{
	#if DEBUG_ACT
		display(0,"set_move_mode(%d)",mode);
	#endif

	move_mode = mode;
	session_act_count = 0;
	cur_act_num = -1;
	ir_mode = 0;
	obs_mode = 0;
	obs_pos = 0;
    session_start = 0;
	act_moved = 0;

}


//--------------------------------------
// act methods
//--------------------------------------

void start_act()
{
	arm::stop();
	lid::stop();
	wheels::stop();
	in_act = false;
	cur_step_num = 0;
	save_act = 0;
	save_step_num = 0;
	loop_count = 0;
	ir_mode = 0;
	obs_mode = 0;
	obs_pos = 0;


	session_start = millis();
		// update the session activity timer

	wheels::setHeading();

	// display the "light" as the act_count % 15 as left to right white user pixels

	#if WITH_PIXELS
		pixels.clear();
		int num = (session_act_count + 1) % 15;
		if (!num)
		{
			for (int i=0; i<5; i++)
			{
				pixels.setPixelColor(PIXEL_USER+i,255,255,255);
			}
		}
		else
		{
			for (int j=0; j<5; j++)
			{
				int pnum = 5-j;
				if (num >= pnum)
				{
					pixels.setPixelColor(PIXEL_USER+j,	255,255,255);
					num -= pnum;
				}
			}
		}
		pixels.show();
	#endif

	session_pass = session_act_count / NUM_ACTS;
	session_act_count++;


	if (session_pass > 0)
	{
		cur_act_num = random(NUM_ACTS);
		if (move_mode && act_moved > 2)
		{
			if (random(4000) > 1000)
				ir_mode = true;
			else
				obs_mode = true;
		}
		else
		{
			ir_mode = random(3000) > 1000;   // 1 in 3 chance of no ir mode
		}
	}
	else
	{
		cur_act_num++;
		if (cur_act_num >= NUM_ACTS) cur_act_num = 0;	// not needed
	}

	cur_act = acts[cur_act_num];
	in_act = true;

	#if DEBUG_ACT
		display(0,"START ACT(%d) sw(%d) ir(%d) arm(%d:%d:%d:%d) lid(%d:%d:%d:%d)",
				cur_act_num,
				switch_state,
				ir_mode,
				arm::busy(),
				arm::attached(),
				arm::moving(),
				arm::read(),
				lid::busy(),
				lid::attached(),
				lid::moving(),
				lid::read());
	#endif
}





void process_act()
{
	if (!in_act)
	{
        // if not in an act, and no activity for a while, time out the session

        #if TIMEOUT_SEESIONS
            if (session_start && (now > session_start + SESSION_TIMEOUT))
            {
				#if DEBUG_ACT
					display(0,"Session Timeout",0);
				#endif
                set_move_mode(0);
				return;
            }
        #endif



		if (ir_mode)
			handle_ir();
		else if (obs_mode)
		{
			int16_t l = 0;
			int16_t r = 0;
			read_ir(&l,&r);

			pixels.setPixelColor(PIXEL_USER+0,l,0,0);
			pixels.setPixelColor(PIXEL_USER+4,r,0,0);
			pixels.show();
			handle_obs();
		}
		return;
	}

	if (arm::busy() || lid::busy() || wheels::busy())
	{
		return;
	}

	step_t *step = 0;


	while (!step)
	{
		step = getStep(cur_act,cur_step_num);

		#if DEBUG_ACT
			display(0,"   got step(%d) type(%d) save_step(%d)",cur_step_num,step->step_type,save_step_num);
		#endif

		if (!step->step_type)
		{
			if (save_act)
			{
				#if DEBUG_ACT
					display(0,"RANDOM END",0);
				#endif
				step = 0;
				cur_act = save_act;
				cur_step_num = save_step_num;
				save_act = 0;
				save_step_num = 0;
			}
			else
			{
				#if DEBUG_ACT
					display(0,"END ACT",0);
				#endif
				in_act = false;
				return;
			}
		}
		else if (step->min_pass > session_pass ||
				step->min_mode > move_mode)
		{
			display(0,"skipping act(%d) step(%d)",cur_act_num,cur_step_num);
			cur_step_num++;
			step = 0;
		}
	}


	uint8_t step_type = step->step_type;

	#if DEBUG_ACT
		display(0," st(%d) sw(%d) ir(%d) loop(%d) arm(%d) lid(%d)",
				cur_step_num,
				switch_state,
				ir_mode,
				loop_count,
				arm::read(),
				lid::read());
	#endif

	if (step_type == STEP_TYPE_MOVE)
	{
		uint32_t bits = step->move.move_bits;

		#if WITH_IR
			if (bits & WAIT_NO_IR)
			{
				int16_t l = 0;
				int16_t r = 0;
				read_ir(&l,&r);
				if ((l > 5) || (r > 5))
				{
					#if DEBUG_ACT
						display(0,"return no_ir",0);
					#endif
					return;
				}
			}
		#endif

		int del = 0;
		if (session_pass == 0 && step->move.delay1)
		{
			del = step->move.delay1 * 10;
		}
		else if (session_pass && step->move.delay_max)
		{
			del = random(step->move.delay_min * 10,step->move.delay_max * 10);
		}
		if (del)
			delay(del);

		uint8_t rate = step->move.rate;
		uint8_t dur = step->move.wheel_duration;

		if (bits & LID_CLOSED)
			lid::close(rate);
		else if (bits & LID_PEEK)
			lid::peek(rate);
		else if (bits & LID_SMALL)
			lid::small(rate);
		else if (bits & LID_OPEN)
			lid::open(rate);

		if (bits & ARM_DOWN)
			arm::down(rate);
		else if (bits & ARM_UP)
			arm::up(rate);
		else if (bits & ARM_OUT)
			arm::out(rate);
		else if (bits & ARM_POISED)
			arm::poised(rate);
		else if (bits & ARM_OFF_POS)
			arm::off_position(rate);
		else if (bits & ARM_TURN_OFF)
			arm::turn_off_switch(rate);

		if (bits & WHEELS_LEFT)
			wheels::left(dur);
		else if (bits & WHEELS_RIGHT)
			wheels::right(dur);
		else if (bits & WHEELS_CW)
			wheels::cw(dur);
		else if (bits & WHEELS_CCW)
			wheels::ccw(dur);
		else if (bits & WHEELS_HOME)
		{
			delay(5);
			wheels::home(true);
		}

	}
	else if (step_type == STEP_TYPE_PIXELS)
	{
		#if WITH_PIXELS
			for (int i=0; i<3; i++)
			{
				pixels.setPixelColor(i+PIXEL_LEFT,step->pixels.lr,step->pixels.lg,step->pixels.lb);
				pixels.setPixelColor(i+PIXEL_RIGHT,step->pixels.rr,step->pixels.rg,step->pixels.rb);
			}
			pixels.show();
		#endif
	}
	else if (step_type == STEP_TYPE_LOOP)
	{
		int count = 0;
		if (session_pass == 0 && step->loop.repeat1)
		{
			count = step->loop.repeat1;
		}
		else if (session_pass && step->loop.repeat_max)
		{
			count = random(step->loop.repeat_min,step->loop.repeat_max+1);
		}
		if (loop_count < count)
		{
			cur_step_num -= step->loop.num_steps;
			loop_count++;
			#if DEBUG_ACT
				display(0,"loop[%d] back to step %d",loop_count,cur_step_num);
			#endif
			return;
		}
		loop_count = 0;
	}

	cur_step_num++;

	// do random acts AFTER bumping step_num to eliminate the opcode

	if (step_type == STEP_TYPE_RANDOM)
	{
		if (random(5000)>2000)
			// only 5 of 9 acts have random moves
			// so to get 1/3 we need to do them 3/5 times
		{
			int tnum = random(NUM_RANDOM_ACTS);
			act_t tact = random_acts[tnum];
			step_t *tstep = getStep(tact,0);

			if (tstep->min_pass <= session_pass && tstep->min_mode <= move_mode)
			{
				#if DEBUG_ACT
					display(0,"RANDOM(%d) save(%d) pass=%d mode=%d",tnum,cur_step_num,tstep->min_pass,tstep->min_mode);
				#endif

				save_act = cur_act;
				save_step_num = cur_step_num;
				cur_act = tact;
				cur_step_num = 0;
				act_moved++;
			}
		}
	}

}


//-----------------------------
// ir handler
//-----------------------------

#define IR_CUTOFF1		5
#define IR_CUTOFF2     15
#define IR_CUTOFF3	   25
#define IR_CUTOFF4     40
#define IR_CUTOFF5	   70


void setInsidePixels(uint8_t r, uint8_t g, uint8_t b)
{
	for (int i=0; i<PIXEL_USER; i++)
		pixels.setPixelColor(i,r,g,b);
	pixels.show();
}

void handle_ir()
{
	if (arm::busy() || lid::busy())
		return;

	static int last_ir = 0;
	static uint32_t ir_up_time = 0;

	int16_t l = 0;
	int16_t r = 0;
	read_ir(&l,&r);

	// display(0,"ir %d %d",l,r);

	int ir = 0;
	if ((l > IR_CUTOFF5) || (r > IR_CUTOFF5))
		ir = 5;
	else if ((l > IR_CUTOFF4) || (r > IR_CUTOFF4))
		ir = 4;
	else if ((l > IR_CUTOFF3) || (r > IR_CUTOFF3))
		ir = 3;
	else if ((l > IR_CUTOFF2) || (r > IR_CUTOFF2))
		ir = 2;
	else if ((l > IR_CUTOFF1) || (r > IR_CUTOFF1))
		ir = 1;

	// moves up happen immediately
	// moves down happen after a second

	uint32_t now = millis();

	if (ir > last_ir || (
		ir < last_ir &&
		now > ir_up_time + 400 ))
	{
		last_ir = ir;
		ir_up_time = now;

		// active IR counts as session activity

		if (ir)
			session_start = now;

		switch (ir)
		{
			case 5:
				setInsidePixels(255,0,0);
				lid::small(128);
				arm::off_position(128);
				break;
			case 4:
				setInsidePixels(200,0,50);
				lid::small(128);
				arm::poised(128);
				break;
			case 3:
				setInsidePixels(180,0,100);
				lid::small(128);
				arm::out(128);
				break;
			case 2:
				setInsidePixels(128,0,100);
				lid::small(128);
				arm::up(128);
				break;
			case 1:
				setInsidePixels(100,0,100);
				lid::peek(128);
				arm::down(128);
				break;
			case 0:
				setInsidePixels(0,0,0);
				lid::close(128);
				arm::down(128);
				break;
		}
	}
}




void handle_obs()
{
	static uint32_t last_obs = 0;
	static int last_dir = 0;

	if (arm::busy() || lid::busy() || wheels::busy())
		return;
	if (millis() < last_obs + 200)
		return;

	#define OBS_BURST  12
	int16_t l = 0;
	int16_t r = 0;
	read_ir(&l,&r);

	if (l<IR_CUTOFF1 && r < IR_CUTOFF1)
		return;

	if (obs_pos < 2 && l > r)
	{
		last_dir = 1;

		obs_pos++;
		wheels::right(OBS_BURST);
		last_obs = millis();
	}
	else if (obs_pos > -2 && r > l)
	{
		last_dir = -1;

		obs_pos--;
		wheels::left(OBS_BURST);
		last_obs = millis();
	}
	else if (l || r)
	{
		if (obs_pos <= -2)
			last_dir = 1;
		else if (obs_pos >= 2)
			last_dir = -1;
		if (last_dir == 1)
		{
			obs_pos++;
			wheels::right(OBS_BURST);
		}
		else
		{
			obs_pos--;
			wheels::left(OBS_BURST);
		}
		last_obs = millis();
	}
	else
		last_dir = 0;
}
