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
