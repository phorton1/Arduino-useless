// acts.cpp

#include "acts.h"
#include "arm.h"
#include "lid.h"
#include "ir.h"
#include "pixels.h"
#include "myDebug.h"


#pragma pack 1



typedef struct
{
	uint8_t	step_type;

	union {
		struct {
			uint16_t 	move_bits;
			uint8_t     pre_delay;		// in 100's of a second
			uint8_t     rate;
			uint8_t		wheel_duration;	// in 100's of a second
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
			uint8_t		num_repeats;
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



step_t normal_medium[] = {
	{ STEP_TYPE_MOVE,	{.move=		{LID_OPEN, 						  0,  50,   0}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_TURN_OFF,					  0,  50,   0}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_DOWN, 						  0,  50,   0}}},
	{ STEP_TYPE_MOVE,	{.move=		{LID_CLOSED, 					  0,  50,   0}}},
	{ 0 }
};


step_t quick_poised[] = {
	{ STEP_TYPE_PIXELS, {.pixels=	{BLUE, BLUE}}},
	{ STEP_TYPE_MOVE,	{.move=		{LID_OPEN, 		  				  0, 255,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{MAGENTA, MAGENTA}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_POISED, 		 			 80, 255,   0}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_TURN_OFF,					100,  10,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{GREEN, GREEN}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_POISED,					  0,  10,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{CYAN, CYAN}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_DOWN, 						100, 255,   0}}},
	{ STEP_TYPE_MOVE,	{.move=		{LID_CLOSED, 			    	  0,  50,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{BLACK, BLACK}}},
	{ 0 }
};



step_t fastest[] = {
	{ STEP_TYPE_PIXELS, {.pixels=	{GREEN, GREEN}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_TURN_OFF,					  0, 255,   0}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_DOWN, 						  0, 255,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{BLACK, BLACK}}},
	{ 0 }
};

step_t quick_small_delayed[] = {
	{ STEP_TYPE_PIXELS, {.pixels=	{RED, RED}}},
	{ STEP_TYPE_MOVE,	{.move=		{LID_SMALL,						100,  30,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{GREEN, GREEN}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_TURN_OFF,		150, 255,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{BLUE, BLUE}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_DOWN, 						  0, 255,   0}}},
	{ STEP_TYPE_MOVE,	{.move=		{LID_CLOSED, 					100,  50,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{BLACK, BLACK}}},
	{ 0 }
};

step_t fastest_no_ir[] = {
	{ STEP_TYPE_PIXELS, {.pixels=	{RED, RED}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_TURN_OFF | WAIT_NO_IR,		  0, 255,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{GREEN, GREEN}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_DOWN, 						  0, 255,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{BLACK, BLACK}}},
	{ 0 }
};


step_t lid_flapper[] = {
	{ STEP_TYPE_PIXELS, {.pixels=	{RED, RED}}},
	{ STEP_TYPE_MOVE,	{.move=		{LID_SMALL, 					  0, 255,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{GREEN, GREEN}}},
	{ STEP_TYPE_MOVE,	{.move=		{LID_CLOSED, 					  0, 255,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{RED, RED}}},
	{ STEP_TYPE_LOOP,   {.loop=		{5,7}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{BLUE, BLUE}}},
	{ STEP_TYPE_MOVE,	{.move=		{LID_SMALL,						100,  50,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{CYAN, CYAN}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_TURN_OFF,					100, 255,   0}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_DOWN, 						  0, 255,   0}}},
	{ STEP_TYPE_MOVE,	{.move=		{LID_CLOSED, 					  0,  50,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{BLACK, BLACK}}},
	{ 0 }
};

step_t arm_repeat[] = {
	{ STEP_TYPE_PIXELS, {.pixels=	{MAGENTA, MAGENTA}}},
	{ STEP_TYPE_MOVE,	{.move=		{LID_OPEN, 						100, 128,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{RED, RED}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_UP, 					     0, 255,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{MAGENTA, MAGENTA}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_POISED,				     0, 255,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{RED, RED}}},
	{ STEP_TYPE_LOOP,   {.loop=		{5,7}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{GREEN, GREEN}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_TURN_OFF,					100, 255,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{BLUE, BLUE}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_DOWN, 						  0, 255,   0}}},
	{ STEP_TYPE_MOVE,	{.move=		{LID_CLOSED, 					  0,  50,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{BLACK, BLACK}}},
	{ 0 }
};


act_t acts[] = {

	normal_medium,
	quick_poised,
	fastest,
	fastest_no_ir,
	quick_small_delayed,
	fastest_no_ir,
	arm_repeat,
	quick_poised,
	fastest,
	fastest_no_ir,
	fastest_no_ir,
	lid_flapper,
};


#define NUM_ACTS	(sizeof(acts)/sizeof(act_t *))




bool in_act = false;
int session_act_count = 0;
int cur_act_num = -1;
act_t cur_act;
int loop_count = 0;
int cur_step_num = 0;




void start_act(bool new_session)
{
	arm::stop();
	lid::stop();

	// a new session goes through the acts in order one time
	// and then starts doing them in a random fashion

	if (new_session)
	{
		session_act_count = 0;
		cur_act_num = -1;
	}


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

	session_act_count++;

	#if 0
		if (session_act_count >= NUM_ACTS)
			cur_act_num = random(NUM_ACTS);
		else
	#endif
	{
		cur_act_num++;
		if (cur_act_num >= NUM_ACTS) cur_act_num = 0;
	}

	cur_act = acts[cur_act_num];

	cur_step_num = 0;
	loop_count = 0;
	in_act = true;
}



void process_act()
{
	if (!in_act)
		return;

	if (arm::busy() || lid::busy())
		return;

	step_t *step = &cur_act[cur_step_num];
	uint8_t step_type = step->step_type;
	if (!step_type)
	{
		in_act = false;
		return;
	}

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
					return;
			}
		#endif

		if (step->move.pre_delay)
		{
			int d = step->move.pre_delay * 10;
			int min = d / 2;
			int max = d + min;
			delay(random(min,max));
		}

		uint8_t rate = step->move.rate;

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
		else if (bits & ARM_TURN_OFF_POSITION)
			arm::off_position(rate);
		else if (bits & ARM_TURN_OFF)
			arm::turn_off_switch(rate);
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
		if (loop_count < step->loop.num_repeats)
		{
			cur_step_num -= step->loop.num_steps;
			loop_count++;
			// display(0,"loop[%d] back to step %d",loop_count,cur_step_num);
			return;
		}
		loop_count = 0;
	}

	cur_step_num++;

}
