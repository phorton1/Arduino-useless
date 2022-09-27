// compass.cpp

#include "compass.h"

#if WITH_COMPASS
    #include <Wire.h>
    #include "HMC5883L.h"
    #include <myDebug.h>
    #include "pixels.h"
    #include <EEPROM.h>

    #define EEPROM_CALIB_OFFSET  32

    static HMC5883L compass;
    static bool compass_mode;
    static int calibration_state;
        // 0 = not calibrating
        // 1 = calibrating
        // 2 = complete
    static uint32_t calibration_time;
    static int16_t last_heading;

    static int16_t compass_x_min;
    static int16_t compass_x_max;
    static int16_t compass_y_min;
    static int16_t compass_y_max;

    static int16_t calib_x_min;
    static int16_t calib_x_max;
    static int16_t calib_y_min;
    static int16_t calib_y_max;

    // Main Methods

    void init_compass()
    {
        Wire.begin();
        compass.initialize();

        // NOTE that you MUST calibrate and save at least one time
        // the default valuse are NOT used!!

        EEPROM.get(EEPROM_CALIB_OFFSET     , compass_x_min);
        EEPROM.get(EEPROM_CALIB_OFFSET + 2 , compass_x_max);
        EEPROM.get(EEPROM_CALIB_OFFSET + 4 , compass_y_min);
        EEPROM.get(EEPROM_CALIB_OFFSET + 6 , compass_y_max);

        display(0,"initialized calibration  min/max x=%d,%d  y=%d,%d",
            compass_x_min,
            compass_x_max,
            compass_y_min,
            compass_y_max);
    }


    bool get_compass_mode()
    {
        return compass_mode;
    }

    void set_compass_mode(bool mode)
    {
        display(0,"set_compass_mode(%d)",mode);
        compass_mode = mode;
        calibration_state = 0;
        last_heading = -100;
    }

    int get_calibration_state()
    {
        return calibration_state;
    }


    int16_t read_compass_heading()
    {
        int16_t mx = 0;
        int16_t my = 0;
        int16_t mz = 0;

        compass.getHeading(&mx, &my, &mz);

        if (calibration_state == 1)
        {
            bool dbg = 0;

            if (mx < calib_x_min) { calib_x_min = mx;  dbg=1; }
            if (mx > calib_x_max) { calib_x_max = mx;  dbg=1; }
            if (my < calib_y_min) { calib_y_min = my;  dbg=1; }
            if (my > calib_y_max) { calib_y_max = my;  dbg=1; }

            if (dbg)
                display(0,"calib(%d,%d,%d,%d)",calib_x_min,calib_x_max,calib_y_min,calib_y_max);
        }

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


    // Calibraton and handle_compass()

    void start_compass_calibration()
    {
        display(0,"start_calibration",0);

        calib_x_min = 32000;
        calib_x_max = -32000;
        calib_y_min = 32000;
        calib_y_max = -32000;

        calibration_state = 1;          // starting
        calibration_time = millis();
    }


    void save_compass_calibration()
    {
        display(0,"SAVING CALIBRATION  min/max x=%d,%d  y=%d,%d",
            compass_x_min,
            compass_x_max,
            compass_y_min,
            compass_y_max);
        if (calibration_state != 2)
        {
            my_error("attempt to save calibration when not ready",0);
            return;
        }

        EEPROM.put(EEPROM_CALIB_OFFSET    ,compass_x_min);
        EEPROM.put(EEPROM_CALIB_OFFSET + 2,compass_x_max);
        EEPROM.put(EEPROM_CALIB_OFFSET + 4,compass_y_min);
        EEPROM.put(EEPROM_CALIB_OFFSET + 6,compass_y_max);

        set_compass_mode(0);
            // exit compass mode upon calibration save
    }


    void handle_compass()
    {
        if (calibration_state == 1)
        {
            read_compass_heading(); // update calibration values

            // calibration for about 30 seconds, or until
            // we have counted to 15 2 seconds at a time

            uint32_t count = (millis() - calibration_time) / 2000;
            static int last_count = -1;
            int use_count = count;
            if (last_count != use_count)
            {
                last_count = use_count;

                #if WITH_PIXELS

                    pixels.clear();
                    for (int j=0; j<5; j++)
                    {
                        int pnum = 5-j;
                        if (use_count >= pnum)
                        {
                            pixels.setPixelColor(PIXEL_USER+j,0,255,255);
                            use_count -= pnum;
                        }
                    }
                    pixels.show();

                #endif

                if (count >= 15)    // calibration complete
                {
                    display(0,"calibration complete",0);
                    calibration_state = 2;
                    last_count = -1;

                    compass_x_min = calib_x_min;
                    compass_x_max = calib_x_max;
                    compass_y_min = calib_y_min;
                    compass_y_max = calib_y_max;

                }
            }
        }

        #if WITH_PIXELS

            else if (!calibration_state)
            {
                // normal compass mode
                //
                // north = blue
                // south = red
                // east  = green
                // west = cyan
                //
                // each pixel is 15 degrees so we really only cover 75 of the 90 degrees.
                // The example below is for the south quadrant which is 136 to 225

                // heading      pixel
                //                0          1          2         3         4
                //               210        195        180       165       150
                // south  (225)218..203   204..189  188..173   172..157  156..141(136)
                //
                //  197                      X
                //  189                      X
                //  188                                 X
                //  225        X
                //  136                                                     X
                //
                //  135   in eastern quadrant
                //  226   in western quadrant

                #define CPIXEL_MIN   10
                #define CPIXEL_INC   5

                int16_t heading = read_compass_heading();
                if (heading == 0) heading = 360;
                    // convert 0 to 360
                    // for comparison below
                if (heading < last_heading - 1  ||
                    heading > last_heading + 1)
                {
                    last_heading = heading;

                    uint8_t r = 0;
                    uint8_t g = 0;
                    uint8_t b = 0;
                    int min_range = 0;

                    if (heading > 45 && heading <= 135)
                    {
                        g = 1;
                        min_range = 51;
                    }
                    else if (heading > 135 && heading <= 225)
                    {
                        b = 1;
                        min_range = 141;
                    }
                    else if (heading > 225 && heading <= 315)
                    {
                        g = 1;
                        b = 1;
                        min_range = 231;
                    }
                    else
                    {
                        if (heading < 180) heading += 360;
                            // changes 1..45 to 361..405
                            // to give continuous range from 316..405
                        r = 1;
                        min_range = 321;
                    }


                    for (int i=0; i<5; i++)
                    {
                        int min_degree = min_range + i * 15;
                        int max_degree = min_degree + 15;
                        int center = min_degree + 7;

                        if (i==5) max_degree += 8;
                        if (i==0) min_degree -= 8;

                        bool on = heading <= max_degree &&
                                  heading >= min_degree;
                        if (on)
                        {
                            int value = abs(heading-center);
                            if (value > 8) value = 8;
                            value = CPIXEL_MIN + (8 - value) * CPIXEL_INC;
                            pixels.setPixelColor(PIXEL_USER+i,value * r,value *g,value *b);
                        }
                        else
                            pixels.setPixelColor(PIXEL_USER+i,0,0,0);
                    }

                    pixels.show();
                }
            }
        #endif  // WITH_PIXELS
    }


#endif  // WITH_COMPASS
