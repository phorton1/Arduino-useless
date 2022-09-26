# Useless Box


- Servo Arm
- Servo Lid
- 4 x Servo Wheels
- LEDs
- IR Sensors
- Compass (for returning after spin move)

## Session Lifecycle

The box initiates a session with someone the first time the switch is thrown.
A session times out after a period (i.e. 1 minute) of activity.


# types of behaviors

- sneaky fast = no lid, fast move to turn off
- slow = open lid, slowly turn off, slowly reverses
- almost, then = move till almost off, wait, then turn off, fast or slow retreat
- jitter = up to including almost position
- lid jitter = down to and including slapping
- waiting = until hand moves away, and/or some amount of time
- moving = left, right, turn left, turn right, rotate, counter-rotate, find front (with compass)
- front lights are generally white on and black off
- inside lights can be synchronized to each move thing and or flash independently
- ir stuff: besides moves, open door, watch, move arm into position
- lock arm


- outside lights can be indicators, i.e. for ir sensors





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
