// compass.h

#pragma once

#include "useless.h"

#if WITH_COMPASS

    extern void init_compass();
    extern int16_t read_compass_heading();

#endif
