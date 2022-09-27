// buttons.h

#pragma once

extern void init_buttons();
extern void handle_buttons();

extern int MOVE_MODE;


// buttons are active for 5 seconds after the first press
//
// BUTTON1
// 		1 - first button press calls restart_session() to start a new session (session timeouts are by #define)
//             flash blue
//      2 - second press goes to MOVE_MODE_1, allowing some evasion and bigger jitters
//             flash green
//      3 - third press goes to MOVE_MODE_2, allowing circles and 2 steps of evasion
//             flash red
//      4 - fourth press puts box into compass mode
//             flash cyan and
//          switch on in compass mode will trigger a calibration cycle where box will circle three times
//          to calibrate the compass.
// BUTTON2
//      controls the LED brightess, storing it in EEPROM.
//      starts as 128, adds 16 per press to get to 255, then goes to 16 again
//      shows white-r-g-b-white
