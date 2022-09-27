
#include "useless.h"
#include <myDebug.h>
#include "pixels.h"
#include "ir.h"
#include "compass.h"
#include "arm.h"
#include "lid.h"
#include "wheels.h"
#include "acts.h"
#include "buttons.h"





bool switch_state;
uint32_t frame_time = 0;





void setup()
{
    Serial.begin(115200);
    display(0,"useless.ino v1.0 started ...",0);

    randomSeed(analogRead(0));

    #if WITH_PIXELS
        init_pixels();  // leaves row of blue showing
    #endif

    init_buttons();

    #if WITH_IR
        init_ir();
    #endif

    #if WITH_COMPASS
        init_compass();
    #endif    init_compass();

    pinMode(PIN_SWITCH,INPUT_PULLUP);

    arm::init();

    lid::init();

    wheels::init();

    #if WITH_PIXELS
        pixels.clear();
        pixels.show();
    #endif
}





void loop()
{
    uint32_t now = millis();
    if (now > frame_time + 30)
    {
        frame_time = now;
        bool sw = digitalRead(PIN_SWITCH) ? 0 : 1;
        if (switch_state != sw)
        {
            switch_state = sw;
            if (sw)
            {
                #if WITH_COMPASS
                    if (get_compass_mode())
                    {
                        start_compass_calibration();
                    }
                    else
                #endif
                {
                    start_act();
                }
            }
            else    // turn off lights
            {
                #if WITH_PIXELS
                    for (int i=0; i<5; i++)
                        pixels.setPixelColor(PIXEL_USER+i,0,0,0);
                    pixels.show();
                #endif

                #if WITH_COMPASS
                    if (get_compass_mode())
                        set_compass_mode(0);
                #endif
            }
        }

        #if WITH_COMPASS
            // acts are not processed in compass mode
            if (get_compass_mode())
                handle_compass();
            else
        #endif

        process_act();

        handle_buttons();
    }

    wheels::update();
        // wheel updating has it's own 1ms frame rate
        // and works during compass_mode
}
