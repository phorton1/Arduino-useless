// compass.cpp

#include "compass.h"


#if WITH_COMPASS
    #include <Wire.h>
    #include "HMC5883L.h"

    #define WITH_CALIBRATION  0

    // true north is +3.4 degrees to the right of magnetic north

    HMC5883L compass;

    static int heading = 237;

    #define DEFAULT_X_MIN  -354
    #define DEFAULT_X_MAX   318
    #define DEFAULT_Y_MIN  -567
    #define DEFAULT_Y_MAX   146

    int16_t compass_x_min = DEFAULT_X_MIN;
    int16_t compass_x_max = DEFAULT_X_MAX;
    int16_t compass_y_min = DEFAULT_Y_MIN;
    int16_t compass_y_max = DEFAULT_Y_MAX;

    #if WITH_CALIBRATION
        #define EEPROM_CALIB_OFFSET  32

        uint32_t in_calibration = 0;
        int16_t calib_x_min = 32000;
        int16_t calib_x_max = -32000;
        int16_t calib_y_min = 32000;
        int16_t calib_y_max = -32000;

        void initCalibration()
        {
            compass_x_min = eepromReadInt(EEPROM_CALIB_OFFSET     , DEFAULT_X_MIN);
            compass_x_max = eepromReadInt(EEPROM_CALIB_OFFSET + 2 , DEFAULT_X_MAX);
            compass_y_min = eepromReadInt(EEPROM_CALIB_OFFSET + 4 , DEFAULT_Y_MIN);
            compass_y_max = eepromReadInt(EEPROM_CALIB_OFFSET + 6 , DEFAULT_Y_MAX);
            display(0,"initialized calibration  min/max x=%d,%d  y=%d,%d",
                compass_x_min,
                compass_x_max,
                compass_y_min,
                compass_y_max);
        }
    #endif


    void init_compass()
    {
        Wire.begin();
        // display(0,"initializing HMC5883 compass ...",0);
        compass.initialize();
        #if WITH_CALIBRATION
            initCalibration();
        #endif
    }

    int16_t read_compass_heading()
    {
        int16_t mx = 0;
        int16_t my = 0;
        int16_t mz = 0;

        compass.getHeading(&mx, &my, &mz);

        #if WITH_CALIBRATION
            // save min and max x/y values invariantly in case
            // we are in calibration
            if (mx < calib_x_min) calib_x_min = mx;
            if (mx > calib_x_max) calib_x_max = mx;
            if (my < calib_y_min) calib_y_min = my;
            if (my > calib_y_max) calib_y_max = my;
        #endif

        // scale x and y and determine heading invariantly

        int16_t tx = map(mx,compass_x_min,compass_x_max,-1200,1200);
        int16_t ty = map(my,compass_y_min,compass_y_max,-1200,1200);
        float f_heading = atan2(ty,tx) * 180.0/PI;

        int16_t ma = f_heading;
        ma -= 90;  // my compass is mounted with Y pointing backwards
        while (ma < 0) ma += 360;
        while (ma > 360) ma -= 360;
        return ma;
    }

#endif
