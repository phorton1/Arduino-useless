
#include "useless.h"
#include <myDebug.h>
#include "pixels.h"
#include "ir.h"
#include "arm.h"
#include "lid.h"
#include "wheels.h"
#include "acts.h"


bool switch_state;
uint32_t frame_time = 0;


void setup()
{
    lid::init();

    Serial.begin(115200);
    delay(200);
    display(0,"useless.ino v1.0 started ...",0);

    randomSeed(analogRead(0));

    #if WITH_PIXELS
        init_pixels();  // leaves row of blue showing
    #endif

    #if WITH_IR
        init_ir();
    #endif

    pinMode(PIN_SWITCH,INPUT_PULLUP);

    arm::init();

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
               start_act();
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

        process_act();

        #if WITH_IR
        #if TEST_IR
            int tl,tr;
            read_ir(&tl,&tr);
            display(0,"TEST_IR(%d,%d)",tl,tr);
        #endif
        #endif
    }

    #if TEST_MODE
        static int what = 0;
        static int count = 0;
        if (Serial.available())
        {
            int c = Serial.read();
            if (c == 'c')       // cw
            {
                display(0,"wheels::cw(%d)",WHEEL_TIME);
                wheels::cw(WHEEL_TIME);
            }
            else if (c == 'w')  // ccw
            {
                display(0,"wheels.ccw(%d)",WHEEL_TIME);
                wheels::ccw(WHEEL_TIME);
            }
            else if (c == 'r')  //  move to right
            {
                display(0,"wheels::right(%d)",WHEEL_TIME);
                wheels::right(WHEEL_TIME);
            }
            else if (c == 'l')  // move to left
            {
                display(0,"wheels::left(%d)",WHEEL_TIME);
                wheels::left(WHEEL_TIME);
            }
            else if (c == '+')  // rotate clockwise
            {
                display(0,"wheels::rotate_cw(%d)",ROTATE_TIME);
                wheels::cw(ROTATE_TIME);
            }
            else if (c == '-')  // rotate counter clockwise
            {
                display(0,"wheels::rotate_ccw(%d)",ROTATE_TIME);
                wheels::ccw(ROTATE_TIME);
            }

        }
    #endif

    wheels::update();
        // wheel updating has it's own 1ms frame rate
        // and works during compass_mode
}
