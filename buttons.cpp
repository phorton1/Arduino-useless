// button.cpp

#include "useless.h"
#include "acts.h"
#include "compass.h"
#include "pixels.h"
#include <myDebug.h>
#include <EEPROM.h>



#define BUTTON_TIMEOUT  2000

typedef struct
{
    bool        last;
    uint32_t    tm;
    int         count;
} button_info;

button_info button[2];




void init_buttons()
{
    pinMode(PIN_BUTTON1,INPUT_PULLUP);
    pinMode(PIN_BUTTON2,INPUT_PULLUP);
}


int handle_button(int num, int pin, uint32_t now)
{
    button_info *info = &button[num];
    if (info->tm && now > info->tm + BUTTON_TIMEOUT)
    {
        info->count = 0;
        info->tm = 0;
        #if WITH_PIXELS
            // save brightness to eeprom after buttons stop
            if (num == 1)
            {
                int brightness = pixels.getBrightness();
                display(0,"saving %d brightness to EEPROM",brightness);
                uint8_t bright = brightness;
                EEPROM.put(EEPROM_BRIGHTNESS,bright);
            }
            #endif
        return -1;
    }
    bool b = !digitalRead(pin);
    if (info->last != b)
    {
        info->last = b;
        if (b)
        {
            info->tm = now;
            info->count++;
            if (num == 0 && info->count > 4) info->count = 1;
                // button 0 wraps 1..4
            display(0,"button[%d] count=%d",num,info->count);
            return info->count;
        }
    }

    return 0;
}



void handle_buttons()
{
    uint32_t now = millis();
    int count1 = handle_button(0,PIN_BUTTON1,now);
    int count2 = handle_button(1,PIN_BUTTON2,now);


    if (count1 > 0) //  && count1 < 5)
    {
        #if WITH_COMPASS
            // if calibration has completed, in compass mode, pressing the button before turning off the switch
            // will cause the calibration to be written to eprom

            if (get_compass_mode() && get_calibration_state() == 2)
            {
                display(0,"button1 pressed while in compass mode and calibration has completed",0);
                #if WITH_PIXELS
                    for (int j=0; j<10; j++)
                    {
                        for (int i=0; i<5; i++)
                            pixels.setPixelColor(PIXEL_USER+i,0,j&1?0:255,j&1?0:255);
                        pixels.show();
                        delay(300);
                    }
                #endif
                save_compass_calibration();
            }

        #endif

        #if WITH_PIXELS
            for (int i=0; i<5; i++)
                pixels.setPixelColor(PIXEL_USER+i,count1==3?255:0,(count1==2||count1==4)?255:0,(count1==1||count1==4)?255:0);
            pixels.show();
            if (count1 == 4)
                delay(1000);
        #endif

        set_move_mode(count1>=4?0:count1-1);
        #if WITH_COMPASS
            set_compass_mode(count1==4);
        #endif
    }
    #if WITH_PIXELS
        else if (count1 < 0)
        {
            pixels.clear();
            pixels.show();
        }

        if (count2 > 0)
        {
            int brightness = pixels.getBrightness();
                // the highest brightness we set is 240,
                // so we can recognize 255 as default (128) in
                // uninitialized eeprom
            brightness += 16;
            if (brightness > 255)
                brightness = 16;
            pixels.setBrightness(brightness);

                // store to eprom

            pixels.setPixelColor(PIXEL_USER+0,255,255,255);
            pixels.setPixelColor(PIXEL_USER+1,255,0,0);
            pixels.setPixelColor(PIXEL_USER+2,0,255,0);
            pixels.setPixelColor(PIXEL_USER+3,0,0,255);
            pixels.setPixelColor(PIXEL_USER+4,255,255,255);
            pixels.show();
        }
        else if (count2 < 0)
        {
            pixels.clear();
            pixels.show();
        }
    #endif
}