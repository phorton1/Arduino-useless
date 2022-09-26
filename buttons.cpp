// button.cpp

#include "useless.h"

bool last_b1 = 0;
bool last_b2 = 0;

void init_buttons()
{
    pinMode(PIN_BUTTON1,INPUT_PULLUP);
    pinMode(PIN_BUTTON2,INPUT_PULLUP);
}

void handle_buttons()
{
    bool b1 = !digitalRead(PIN_BUTTON1);
    if (last_b1 != b1)
    {
        last_b1 = b1;
        if (b1)
        {
            extern void restart_session();
            restart_session();
        }
    }
}