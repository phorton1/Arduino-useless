
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


bool switch_state;
uint32_t frame_time = 0;
uint32_t session_start = 0;




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



void restart_session()
{
    extern bool ir_mode;    // in acts.cpp
    ir_mode = false;

    #if WITH_PIXELS
        for (int i=0; i<5; i++)
            pixels.setPixelColor(PIXEL_USER+i,255,0,0);
        pixels.show();
        delay(1000);
        pixels.clear();
        pixels.show();
    #endif

    session_start = 0;
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
                bool new_session = session_start == 0 ? 1 : 0;
                session_start = now;
                start_act(new_session);
            }
            else    // turn off lights
            {
                #if WITH_PIXELS
                    for (int i=0; i<5; i++)
                        pixels.setPixelColor(PIXEL_USER+i,0,0,0);
                    pixels.show();
                #endif
            }
        }

        // time out session flashing red once

        #if 0
            if (session_start && (now > session_start + SESSION_TIMEOUT))
            {
                restart_session();
            }
        #endif

        process_act();

        handle_buttons();

        // PRH - need compass calibration!
        // display(0,"heading=%d",read_compass_heading());
    }

    wheels::update();
}
