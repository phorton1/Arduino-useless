# Useless Box - Software

**[Home](readme.md)** --
**[Electronics](electronics.md)** --
**[Wood](wood.md)** --
**[Top](top.md)** --
**[Bottom](bottom.md)** --
**Software**


One early discovery was that WS2812b LEDs are basically completely incompatible
with most Arduino servo libraries.  If you change the LEDs while the servos are
running, interrupts get turned off while writing to the WS2812s, and the servos
jitter and jump around. After quite a bit of wrangling with the issue, I finally
just gave up and coded the program so that the servos are never running when the
LEDs are changed, and vice-versa.

## Installation/Building

Use **git** or download a **zip** of this repository and place it's contents
in an Arduino *sketch folder* called **useless**.

You will also **require** the following libraries in your Arduino IDE environment

- **[myDebug](https://github.com/phorton1/Arduino-libraries-myDebug)** -
my arduino/teensy/esp32 debug output routines, installed by using **git** or manually installing from
the **[git_repository](https://github.com/phorton1/Arduino-libraries-myDebug)**
- **VarSpeedServo** - installed via the Arduino library manager
- **Adafruit_NeoPixel** - installed via the Arduino library manager

Open the **useless.ino** sketch in the Arduino IDE, then compile and
upload it to the **Arduino Nano**.


## Design

There are many design details, but here I am only going to briefly describe
the **behavioral** aspects of the design.

- There are a set of **acts**.
- The number of acts determines the length of a **pass**.
- Acts made up of **steps**.
- Steps are assigned a minium **pass** and minimum **mode** they take place in.
- The times, and some of the parameters of an act can be **randomized** between defined limits.

On the first *pass*, the program will go through all the *acts* in order one time.
Starting on the second pass, *acts* are chosen randomly and randomization of parameters begins.
On the second pass **IR mode** (raising the arm as the user's finger gets close to the switch) also begins, randomly, between acts.
Starting on the third pass, **1st level movements steps** within acts (jiggles and shakes) are turned on and once two of those have happened,
we start allowing **full rotations** to occur.  Also at that point we start interpersing random **avoidance modes** along
with the IR mode behavior.


**Please see the source code for more information**


## User Interface

There are two buttons that can be pressed with toothpicks, button1 is towards
the front of the box, and button2 is towards the back.  In software the button
numbers are zero based.

The front button1 controls the brightness of the LED. Pressing it will cycle
through brighter and brighter LEDs until it starts at dim again.  Once you
stop pressing this button, the brightness value is saved to EEPROM for
subsequent reboots.

The rearward button2 is intended to provide user controlable modal behavior
to the box by allowing the user to specify greater (or lesser) modes of
chaos, enabling certain "acts" or steps within acts to be performed at
a given mode.

The idea behind this is that perhaps you want to leave the box sitting
on a cluttered shelf, and don't want the wheels, by default, to move the
box.  But then you might want to allow the user to "turn on" the wheels
by pressing button2 two times to go to mode2. The wheels might only jiggle
back and forth, which is very unlikely to fly off a table, and then a
separate mode3, which would allow it to fully rotate, which is much more
likely for it to fly off the table.

**The current implementation of button2 is kind of wonky**.  I discovered
I generally *want* the box to do all of the acts by default, so it boots
up in *Mode1* with all movements basically turned on.    Pressing the
button once, would then set it to *Mode0* where no movements should take
place.

The (wonky) implementation also had a fixed number of modes ... three (3) ...
and the forth press of the button puts the box into "compass calibration
mode" if the program has been compiled with WITH_COMPASS=1 defined.
When it goes into calibration mode, you spin it slowly three times
until a pattern of lights occurs. IF YOU THEN THROW THE MAIN SWTICH
ON THE TOP OF THE BOX the compass calibration will be saved to EEPROM.

Sheesh.  What a weird UI !!!

And, as I said, I found the compass idea only worked so-so to get the
box to rotate and return to it's starting position.  That feature should
generally be considered *experimental*.

In any case, after NOT pressing a button for 2 seconds, the user interface
starts over, waiting for a button press.

## Closing

I have not done a very good job of documenting this project.

If you are interested in more details, please contact me through github.

Otherwise, I *may* revisit this documentation again someday, but until then ...


**Done!!** [**Back**](readme.md) to the beginning ...
