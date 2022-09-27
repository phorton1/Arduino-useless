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
	{ STEP_TYPE_MOVE,	{.move=		{ARM_DOWN, 						  0, 255,   0}}},
	{ STEP_TYPE_MOVE,	{.move=		{LID_OPEN, 						  0,  50,   0}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_TURN_OFF,					  0,  50,   0}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_DOWN, 						  0,  50,   0}}},
	{ STEP_TYPE_MOVE,	{.move=		{LID_CLOSED, 					  0,  50,   0}}},
	{ 0 }
};


step_t quick_poised[] = {
	{ STEP_TYPE_MOVE,	{.move=		{ARM_DOWN, 						  0, 255,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{BLUE, BLUE}}},
	{ STEP_TYPE_MOVE,	{.move=		{LID_OPEN, 		  				  0, 255,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{MAGENTA, MAGENTA}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_POISED, 		 			 80, 255,   0}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_TURN_OFF,					100, 128,   0}}},
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
	{ STEP_TYPE_MOVE,	{.move=		{ARM_DOWN | LID_CLOSED, 		  0, 255,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{BLACK, BLACK}}},
	{ 0 }
};

step_t quick_small_delayed[] = {
	{ STEP_TYPE_PIXELS, {.pixels=	{RED, RED}}},
	{ STEP_TYPE_MOVE,	{.move=		{LID_SMALL,						100,  30,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{GREEN, GREEN}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_TURN_OFF,					150, 255,   0}}},
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
	{ STEP_TYPE_MOVE,	{.move=		{ARM_DOWN | LID_CLOSED, 		  0, 255,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{BLACK, BLACK}}},
	{ 0 }
};


// flapper explicitly moves ARM_DOWN and LID_CLOSED to begin in case of ir_mode interrupt

step_t lid_flapper[] = {
	{ STEP_TYPE_MOVE,	{.move=		{ARM_DOWN | LID_CLOSED,			  0, 255,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{GREEN, RED}}},
	{ STEP_TYPE_MOVE,	{.move=		{LID_SMALL, 					  0, 255,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{GREEN, RED}}},
	{ STEP_TYPE_MOVE,	{.move=		{LID_CLOSED, 					  0, 255,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{RED, GREEN}}},
	{ STEP_TYPE_LOOP,   {.loop=		{5,10}}},
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
	{ STEP_TYPE_LOOP,   {.loop=		{5,10}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{GREEN, GREEN}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_TURN_OFF,					100, 255,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{BLUE, BLUE}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_DOWN, 						  0, 255,   0}}},
	{ STEP_TYPE_MOVE,	{.move=		{LID_CLOSED, 					  0,  50,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{BLACK, BLACK}}},
	{ 0 }
};

step_t middle_flapper[] = {
	{ STEP_TYPE_MOVE,	{.move=		{ARM_DOWN | LID_CLOSED,			  0, 255,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{GREEN, RED}}},
	{ STEP_TYPE_MOVE,	{.move=		{LID_SMALL, 					  0, 255,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{GREEN, RED}}},
	{ STEP_TYPE_MOVE,	{.move=		{LID_CLOSED, 					  0, 255,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{RED, GREEN}}},
	{ STEP_TYPE_LOOP,   {.loop=		{5,8}}},

	{ STEP_TYPE_PIXELS, {.pixels=	{BLUE, BLUE}}},
	{ STEP_TYPE_MOVE,	{.move=		{LID_SMALL,						  0, 255,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{CYAN, CYAN}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_TURN_OFF,					  0, 255,   0}}},
	{ STEP_TYPE_MOVE,	{.move=		{ARM_DOWN | LID_CLOSED, 		  0, 255,   0}}},

	{ STEP_TYPE_PIXELS, {.pixels=	{GREEN, RED}}},
	{ STEP_TYPE_MOVE,	{.move=		{LID_SMALL, 					  0, 255,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{GREEN, RED}}},
	{ STEP_TYPE_MOVE,	{.move=		{LID_CLOSED, 					  0, 255,   0}}},
	{ STEP_TYPE_PIXELS, {.pixels=	{RED, GREEN}}},
	{ STEP_TYPE_LOOP,   {.loop=		{5,8}}},

	{ STEP_TYPE_PIXELS, {.pixels=	{BLACK, BLACK}}},
	{ 0 }
};


//--------------------------------------
// list of acts
//--------------------------------------


act_t acts[] = {
	normal_medium,
	quick_poised,
	fastest,
	fastest_no_ir,
	quick_small_delayed,
	fastest_no_ir,
	arm_repeat,
	quick_poised,
	lid_flapper,
	fastest,
	fastest_no_ir,
	fastest_no_ir,
	middle_flapper,
	fastest,
	fastest_no_ir,
};


#define NUM_ACTS	(sizeof(acts)/sizeof(act_t *))



//--------------------------------------
// global variables
//--------------------------------------

bool in_act = false;
int session_act_count = 0;
int cur_act_num = -1;
act_t cur_act;

int loop_count = 0;
int cur_step_num = 0;

bool ir_mode = false;


void handle_ir();	// forward



//--------------------------------------
// act methods
//--------------------------------------

void start_act(bool new_session)
{
	// a new session goes through the acts in order one time
	// and then starts doing them in a random fashion

	if (new_session)
	{
		session_act_count = 0;
		cur_act_num = -1;
		ir_mode = 0;
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

	if (session_act_count > 15)
	{
		int num =session_act_count - 10;
		ir_mode = random(num) > 2;
	}

	#if 1
		if (session_act_count > NUM_ACTS)
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

	#if 0
		display(0,"act(%d) sw(%d) ir(%d) arm(%d:%d:%d:%d) lid(%d:%d:%d:%d)",
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
		if (ir_mode)
			handle_ir();
		return;
	}

	if (arm::busy() || lid::busy())
	{
		return;
	}

	step_t *step = &cur_act[cur_step_num];
	uint8_t step_type = step->step_type;
	if (!step_type)
	{
		in_act = false;
		return;
	}

	#if 0
		display(0," st(%d) sw(%d) ir(%d) arm(%d:%d:%d:%d) lid(%d:%d:%d:%d)",
				cur_step_num,
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
					// display(0,"return no_ir",0);
					return;
				}
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
		if (loop_count < 2 + random(step->loop.num_repeats))
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

		switch (ir)
		{
			case 5:
				setInsidePixels(255,0,0);
				lid::small(128);
				arm::poised(128);
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
