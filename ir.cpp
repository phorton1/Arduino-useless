// ir.cpp

#include "ir.h"

#if WITH_IR

	#define IR_FUDGE  50

	int ambient_l = 0;
	int ambient_r = 0;


	void init_ir()
	{
		pinMode(PIN_L_SENSE,INPUT);
		pinMode(PIN_L_LED,OUTPUT);
		pinMode(PIN_R_SENSE,INPUT);
		pinMode(PIN_R_LED,OUTPUT);

		digitalWrite(PIN_L_LED,1);
		digitalWrite(PIN_R_LED,1);
		delay(15);

		ambient_l = analogRead(PIN_L_SENSE);
		ambient_r = analogRead(PIN_R_SENSE);
		digitalWrite(PIN_L_LED,0);
		digitalWrite(PIN_R_LED,0);
	}


	void read_ir(int16_t *l, int16_t *r)
	{
		digitalWrite(PIN_L_LED,1);
		digitalWrite(PIN_R_LED,1);
		delay(2);
		int16_t il = analogRead(PIN_L_SENSE);
		int16_t ir = analogRead(PIN_R_SENSE);
		il = map(il,ambient_l+IR_FUDGE,1023,0,255);
		ir = map(ir,ambient_r+IR_FUDGE,1023,0,255);
		if (il < 0) il = 0;
		if (ir < 0) ir = 0;
		*l = il;
		*r = ir;
	}

#endif
