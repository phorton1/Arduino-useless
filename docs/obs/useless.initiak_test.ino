
#include "useless.h"
#include <myDebug.h>

// OVERIDING DESIGN CONSIDERATION
//
// Calling neoPixels.show() while servos are attached causes jitter in the servos.
//
// Therefor care must be taken to coordinate the movement of servos with pixel changes.
//
// In general we keep the servos detached when not in use.
// The only place that *might* be a problem is holding up a spring loaded lid.
//
// Jitter is unimportant in the wheels.


#define WITH_PIXELS     1
#define WITH_COMPASS    1
#define WITH_SERVOS     1


#if WITH_PIXELS
    #include "Adafruit_NeoPixel.h"
    #define NUM_PIXELS   11
    Adafruit_NeoPixel pixels(NUM_PIXELS, PIN_LED_STRIP, NEO_GRB + NEO_KHZ800);
#endif


#if WITH_COMPASS
    #include <Wire.h>
    #include "HMC5883L.h"

    #define WITH_CALIBRATION  0

    #define BOCAS_DECLINATION  3.4
    #define BOCAS_NORTH 9.4048
    #define BOCAS_WEST  82.2692

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
#endif




#if WITH_SERVOS
    #include <Servo.h>

    typedef struct
    {
        uint8_t servo_num;
        uint8_t degrees;
    } servo_scene;

    int last_button1 = 0;

    int scene = 0;
    Servo servo[NUM_SERVOS];
    servo_scene scenes[] = {

        { SERVO_ARM,        90 },
        { SERVO_ARM,        0 },
        { SERVO_LID,        50},
        { SERVO_LID,        0 },
        #if 1
            { SERVO_WHEEL_FL,   180 },
            { SERVO_WHEEL_FL,   0 },
            { SERVO_WHEEL_FL,   90 },
            { SERVO_WHEEL_BL,   180 },
            { SERVO_WHEEL_BL,   0 },
            { SERVO_WHEEL_BL,   90 },
            { SERVO_WHEEL_BR,   180 },
            { SERVO_WHEEL_BR,   0 },
            { SERVO_WHEEL_BR,   90 },
            { SERVO_WHEEL_FR,   180 },
            { SERVO_WHEEL_FR,   0 },
            { SERVO_WHEEL_FR,   90 },
        #endif
    };

    #define NUM_SCENES  (sizeof(scenes) / sizeof(servo_scene))

    uint8_t servo_pins[] = {
      PIN_ARM,
      PIN_LID,
      PIN_WHEEL_FL,
      PIN_WHEEL_BL,
      PIN_WHEEL_BR,
      PIN_WHEEL_FR,
      // PIN_UNUSED_SERVO,
    };
#endif



int ambient_l = 0;
int ambient_r = 0;

void setup()
{
    Serial.begin(115200);

    #if WITH_PIXELS
        pixels.begin();
        pixels.setBrightness(128);
        pixels.clear();
        pixels.show();
        for (int j=0; j<3; j++)
        {
            for (int i=0; i<NUM_PIXELS; i++)
            {
                delay(50);
                pixels.setPixelColor(i,
                    j==0?255:0,
                    j==1?255:0,
                    j==2?255:0);
                pixels.show();
            }
            delay(100);
        }
        delay(200);
        pixels.clear();
        pixels.show();
    #endif

    pinMode(PIN_L_SENSE,INPUT);
    pinMode(PIN_L_LED,OUTPUT);
    digitalWrite(PIN_L_LED,1);

    pinMode(PIN_R_SENSE,INPUT);
    pinMode(PIN_R_LED,OUTPUT);
    digitalWrite(PIN_R_LED,1);

    delay(15);
    ambient_l = analogRead(PIN_L_SENSE);
    ambient_r = analogRead(PIN_R_SENSE);
    digitalWrite(PIN_L_LED,0);
    digitalWrite(PIN_R_LED,0);

    pinMode(PIN_SWITCH,INPUT_PULLUP);
    pinMode(PIN_BUTTON1,INPUT_PULLUP);
    pinMode(PIN_BUTTON2,INPUT_PULLUP);

    #if WITH_SERVOS
        for (int i=0; i<NUM_SERVOS; i++)
        {
            servo[i].attach(servo_pins[i]);
            servo[i].write(i>SERVO_LID?90:0);
            // delay(100);
            // servo[i].detach();
        }
    #endif

    #if WITH_COMPASS
        Wire.begin();
        display(0,"initializing HMC5883 compass ...",0);
        compass.initialize();
        #if WITH_CALIBRATION
            initCalibration();
        #endif
    #endif
}



void loop()
{
    uint32_t now = millis();
    static uint32_t frame_time = 0;

    if (now > frame_time + 30)
    {
        frame_time = now;
        digitalWrite(PIN_L_LED,1);
        digitalWrite(PIN_R_LED,1);
        delay(1);

        int16_t heading = 0;

        #if WITH_COMPASS
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
            heading = ma;
        #endif


        int ir_left = analogRead(PIN_L_SENSE);
        int ir_right = analogRead(PIN_R_SENSE);
        int sw = digitalRead(PIN_SWITCH)?0:500;
        int button1 = digitalRead(PIN_BUTTON1)?0:500;
        int button2 = digitalRead(PIN_BUTTON2)?0:500;
        digitalWrite(PIN_L_LED,0);
        digitalWrite(PIN_R_LED,0);
        display(0,"%d,%d,%d,%d,%d,%d",sw,button1,button2,heading,ir_left,ir_right);

        #if WITH_PIXELS
            #define TEMP_IR_MIN    400

            int16_t lp = map(ir_left,   ambient_l+50,1023,   0,255);
            int16_t rp = map(ir_right,  ambient_r+50,1023,   0,255);
            if (lp < 0) lp = 0;
            if (rp < 0) rp = 0;


            bool show_leds = false;
            static int16_t last_lp = 0;
            static int16_t last_rp = 0;

            if (last_lp != lp)
            {
                last_lp = lp;
                pixels.setPixelColor(6, lp, 0, 0);
                show_leds = true;
            }
            if (last_rp != rp)
            {
                last_rp = rp;
                pixels.setPixelColor(10, rp, 0, 0);
                show_leds = true;
            }
            if (show_leds)
                pixels.show();

        #endif

        #if WITH_SERVOS
            if (button1 != last_button1)
            {
                last_button1 = button1;
                if (button1)
                {
                    // servo[scenes[scene].servo_num].detach();
                    scene++;
                    if (scene >= NUM_SCENES) scene=0;
                    int servo_num = scenes[scene].servo_num;
                    int degrees = scenes[scene].degrees;
                    int pin = servo_pins[servo_num];
                    // display(0,"scene(%d)  servo[%d].write(%d)   pin(%d)",scene,servo_num,degrees,pin);
                    // servo[servo_num].attach(servo_pins[servo_num]);
                    servo[servo_num].write(degrees);
                }
            }
        #endif
    }
}