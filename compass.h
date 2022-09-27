// compass.h

#pragma once

#include "useless.h"

#if WITH_COMPASS

    extern void init_compass();
    extern int16_t read_compass_heading();

    extern void set_compass_mode(bool mode);
    extern bool get_compass_mode();

    extern void handle_compass();
    extern void start_compass_calibration();
    extern int get_calibration_state();
    extern void save_compass_calibration();

#endif
