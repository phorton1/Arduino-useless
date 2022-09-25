
#include "useless.h"
#include <myDebug.h>
#include "buttons.h"
#include "compass.h"
#include "ir.h"
#include "pixels.h"


#define VSERVOS   1

#if VSERVOS
    #include <VarSpeedServo.h>
    VarSpeedServo servo[NUM_SERVOS];
#else
    #include <Servo.h>
    Servo servo[NUM_SERVOS];
#endif


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

    #if VSERVOS
        if (now > frame_time + 30)
        {
            frame_time = now;

            bool sw = !digitalRead(PIN_SWITCH);
            if (switch_on != sw)
            {
                #define SPEED 0

                switch_on = sw;
                if (sw)
                {
                    #if WITH_PIXELS
                        for (int i=0; i<5; i++)
                        {
                            pixels.setPixelColor(i+PIXEL_USER,255,255,255);
                        }
                        pixels.show();
                    #endif

                    servo[0].attach(servo_pins[0]);
                    servo[1].attach(servo_pins[1]);
                    // servo[1].write(45,SPEED,true);
                    servo[0].write(92,SPEED,false);
                }
                else
                {
                    #if WITH_PIXELS
                        for (int i=0; i<5; i++)
                        {
                            pixels.setPixelColor(i+PIXEL_USER,0,0,0);
                        }
                        pixels.show();
                    #endif
                    servo[0].write(0,SPEED,false);
                }
            }
            else if (!sw)
            {
                if (servo[0].attached() &&
                    servo[0].read() == 0)
                {
                    servo[0].detach();
                    servo[1].write(0,SPEED);
                }
                if (servo[1].attached() &&
                    servo[1].read() == 0)
                    servo[1].detach();
            }

    #else

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
                         servo0_pos >= 92)
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
                    float pct = diff / 500.0;
                    if (!direction) pct = 1 - pct;
                    if (pct < 0) pct = 0;
                    if (pct > 100) pct = 100;

                    float s0 = pct * 92;
                    float s1 = pct * 45;
                    int i0 = s0;
                    int i1 = s1;

                    #if 0
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
        #endif


        // other stuff

        #if WITH_IR
            int16_t ir_left = 0;
            int16_t ir_right = 0;
            static int16_t last_left = 0;
            static int16_t last_right = 0;
            read_ir(&ir_left,&ir_right);

            display(0,"%d %d",ir_left,ir_right);

            static int position = 0;
            static bool in_move = 0;


                bool show_leds = false;
                int value1 = 0;
                int value2 = 0;

                if (last_left != ir_left)
                {

                    last_left = ir_left;
                    #if WITH_PIXELS
                        pixels.setPixelColor(6, ir_left, 0, 0);
                    #endif
                    show_leds = true;

                    if (!in_move && ir_left > 4 && position <= 0)
                    {
                        display(0,"moving to right",0);
                        in_move = 1;
                        value1 = 10;
                        value2 = 170;
                        position++;
                    }
                }
                if (last_right != ir_right)
                {
                    last_right = ir_right;
                    #if WITH_PIXELS
                        pixels.setPixelColor(10, ir_right, 0, 0);
                    #endif
                    show_leds = true;
                    if (!in_move && ir_right > 4 && position >= 0)
                    {
                        display(0,"moving to left",0);
                        in_move = 1;
                        value1 = 170;
                        value2 = 10;
                        position--;;
                    }
                }
                #if WITH_PIXELS
                    if (show_leds)
                       pixels.show();
                #endif

                if (0 && value1)
                {
                    for (int i=0; i<4; i++)
                    {
                        servo[i+2].attach(servo_pins[i+2]);
                    }

                    servo[2].write(value1);
                    servo[3].write(value2);
                    servo[4].write(value2);
                    servo[5].write(value1);

                    delay(150);

                    for (int i=0; i<4; i++)
                    {
                        servo[i+2].write(90);
                    }
                    delay(20);
                    for (int i=0; i<4; i++)
                    {
                        servo[i+2].detach();
                    }
                    display(0,"done with move",0);
                    in_move = 0;
                }

            #endif        #endif


        #if WITH_COMPASS
            int16_t heading = read_compass_heading();
        #endif

        handle_buttons();

    }
}