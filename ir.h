// ir.h

#pragma once

#include "useless.h"

#if WITH_IR

	extern void init_ir();

	extern void read_ir(int16_t *l, int16_t *r);	// returns 0..255 in aigned integer to allow for math

#endif
