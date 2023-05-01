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
- Steps are assigned a minium **pass** they take place in.
- The times, and some of the parameters of an act can be **randomized** between defined limits.

On the first *pass*, the program will go through all the *acts* in order one time.
Starting on the second pass, *acts* are chosen randomly and randomization of parameters begins.
On the second pass **IR mode** (raising the arm as the user's finger gets close to the switch) also begins, randomly, between acts.
Starting on the third pass, **1st level movements steps** within acts (jiggles and shakes) are turned on and once two of those have happened,
we start allowing **full rotations** to occur.  Also at that point we start interpersing random **avoidance modes** along
with the IR mode behavior.


**Please see the source code for more information**


## Closing

I have not done a very good job of documenting this project.

If you are interested in more details, please contact me through github.

Otherwise, I *may* revisit this documentation again someday, but until then ...


**Done!!** [**Back**](readme.md) to the beginning ...
