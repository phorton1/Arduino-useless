
#include "useless.h"
#include <myDebug.h>
#include <Servo.h>
#include "buttons.h"
#include "compass.h"
#include "ir.h"
#include "pixels.h"

// servo pins are re-directed through an array
// even though by default they are 2..7, so I
// can change them if the arduino breaks !?!

uint8_t servo_pins[] = {
  PIN_ARM,
  PIN_LID,
  PIN_WHEEL_FL,
  PIN_WHEEL_BL,
  PIN_WHEEL_BR,
  PIN_WHEEL_FR,
  // PIN_UNUSED_SERVO,
};


Servo servo[NUM_SERVOS];



void setup()
{
    Serial.begin(115200);
    display(0,"useless.ino v1.0 started ...",0);

    #if WITH_PIXELS
        init_pixels();
        setLeftPixels(255,0,0);
    #endif

    pinMode(PIN_SWITCH,INPUT_PULLUP);
    for (int i=0; i<NUM_SERVOS; i++)
    {
        servo[i].attach(servo_pins[i]);
        servo[i].write(i>SERVO_LID?90:0);
        #if WITH_PIXELS
            delay(100);
            pixels.setPixelColor(PIXEL_USER + i,0,0,255);
            pixels.show();
        #endif
        delay(100);
        servo[i].detach();
    }

    #if WITH_IR
        init_ir();
    #endif

    #if WITH_COMPASS
        init_compass();
    #endif    init_compass();

    #if WITH_PIXELS
        delay(100);
        setRightPixels(255,0,0);
        delay(500);
        pixels.clear();
        pixels.show();
    #endif
}



bool switch_on = 0;

uint32_t cycle_start = 0;
bool direction = 0;

bool attached = false;
int servo0_pos = 0;
int servo1_pos = 0;


#define CYCLE_DELAY  2000


void loop()
{
    uint32_t now = millis();
    static uint32_t frame_time = 0;

    if (now > frame_time + 30)
    {
        frame_time = now;

        bool sw = !digitalRead(PIN_SWITCH);
        if (switch_on != sw)
        {
            switch_on = sw;
            display(0,"switch=%d",sw);
            cycle_start = now;
            direction = sw ? 1 : 0;
        }

        else if (cycle_start)
        {
            if (!direction &&
                servo0_pos <= 0 &&
                servo1_pos <= 0)
            {
                display(0,"done",0);
                cycle_start = 0;
                servo0_pos = 0;
                servo1_pos = 0;
                attached = false;
                servo[0].detach();
                servo[1].detach();
                #if WITH_PIXELS
                    pixels.clear();
                    pixels.show();
                #endif
            }
            else if (direction &&
                     servo0_pos >= 90)
            {
                display(0,"at 90",0);
                cycle_start = now;
                direction = 0;
            }
            else if (now > cycle_start + CYCLE_DELAY)
            {
                if (!attached)
                {
                    attached = true;
                    servo[1].attach(servo_pins[1]);
                    servo[0].attach(servo_pins[0]);
                }
                float diff = now - cycle_start - CYCLE_DELAY;
                float pct = diff / 100.0;
                if (!direction) pct = 1 - pct;

                float s0 = pct * 90;
                float s1 = pct * 35;
                int i0 = s0;
                int i1 = s1;

                #if 1
                    Serial.print(diff);
                    Serial.print(" ");
                    Serial.print(pct,3);
                    Serial.print(" ");
                    Serial.print(i0);
                    Serial.print(" ");
                    Serial.println(i1);
                #endif

                #if WITH_PIXELS
                    float leds = 128 * pct;
                    for (int i=0; i<NUM_PIXELS; i++)
                    {
                        if (i != 6 && i != 10)
                        pixels.setPixelColor(i,0,0,leds);
                    }
                    pixels.show();
                #endif

                if (servo0_pos != i0)
                {
                    servo0_pos = i0;
                    servo[0].write(servo0_pos);
                }
                if (servo1_pos != i1)
                {
                    servo1_pos = i1;
                    servo[1].write(servo1_pos);
                }
            }
        }


        // other stuff

        #if WITH_IR
            int16_t ir_left = 0;
            int16_t ir_right = 0;
            static int16_t last_left = 0;
            static int16_t last_right = 0;
            read_ir(&ir_left,&ir_right);

            #if WITH_PIXELS
                bool show_leds = false;
                if (last_left != ir_left)
                {
                    last_left = ir_left;
                    pixels.setPixelColor(6, ir_left, 0, 0);
                    show_leds = true;
                }
                if (last_right != ir_right)
                {
                    last_right = ir_right;
                    pixels.setPixelColor(10, ir_right, 0, 0);
                    show_leds = true;
                }
                if (show_leds)
                    pixels.show();
            #endif
        #endif


        #if WITH_COMPASS
            int16_t heading = read_compass_heading();
        #endif

        handle_buttons();

    }
}