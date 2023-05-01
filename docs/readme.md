# Useless Box

**Home** --
**[Electronics](electronics.md)** --
**[Wood](wood.md)** --
**[Top](top.md)** --
**[Bottom](bottom.md)** --
**[Software](software.md)**

For the fun of it, I mada a **Useless Box**.

**Insert YouTube Demo movie here**

All of the **fusion360 files** and the **STL** and **DXF** files exported from fusion, as well as
the **Prusa Slicer** and **Gcode** files and **Lightburn** projects I then used to print the
plastic parts and laser cut the wood can be found in the following subdirecties in the **docs** folder

- **fusion** - the fusion 360 design files
- **stl** - the STL files that I used to slice the plastic parts
- **dxf** - the DXF files that I used to laser cut the wood parts
- **prusa** - the Prusa slicer files and resultant gcode I used to print the plastic parts
- **lightburn** - the Lightburn projects I used to cut the wood.

## Design

I decided on the following basic set of requirements for my useless box

- random **behaviors** - it will be controlled by an arduino nano with various random behaviors
- **mobility** - it will have motors and semi-hidden wheels so it can spin, move left and right, etc
- **sensors** - it will have sensors, probably infrared, to detect when a human tries to touch it
- **leds** - it will have a string of WD2812b leds that will light up the interior for effect and probably some on the top to show the user
- **on-off switch** - it will have an on/off switch to the power supply
- **batteries** - it will use rechargable 18650 Lithium batteries, because I have a bunch of those, and pre-made plastic holders for them
- **nicely constructed** - it will look nice, like a bit of a wood working effort

I had recently upgraded my [**home built cnc machine**](https://github.com/phorton1/Arduino-esp32_cnc20mm) with a new 10W laser, and
had been having a lot of fun making wooden boxes with it, and this seemed like a perfect opportunity to combine that with
some electronics and programming to make something interesting.  I had a bunch of inexpensive **3/16" plywood** that I had
become proficient in cutting and glueing, so I decided the box would be made out of that.

I decided to use small MG90 Servos.  I tested them against a dual 3.7V (7.4V total) 18650 battery pair and, although they get hot
if left running continuously, for short bursts they work ok, and in my application they would not need to be kept running or
holding any positions against any serious forces.  The extra voltage (they are rated at 5V) resulted in extra torque so I felt
that I did not need the much larger kinds of servos that other similar box makers seemed to insist on using.

I also knew that I had bought a number of of "continuous" MG90 servos (by mistake) for an earlier project.  Those turned out to
be perfect for running the motors for the wheels.  I can easily control which way, as well as the speed, each wheel turns with a fair degree
of accuracy.

I decided I would 3D print wheels that made use of some 30mm rubber O-rings that I had.
![gif2.gif](images/gif2.gif)

I used one piece of **1/8" plywood** for the partition.

![all_parts_made.jpg](images/all_parts_made.jpg)

### Bill of Materials

- one SPDT blue switch and nut for the top
- one slider SPST switch for the power supply
- a 2 cell 18650 battery holder
- two 90 degree MG90 servos and screws
- four continuous MG90 servos and screws
- four 30x2mm rubber O-rings
- ws2812B LED strip
- two M2x12 screws for the back
- one M3x12 screw for the front
- four or five small screws to hold the main circuit board
- two four wired JST connectors for the sensors
- two two wired JST connectors, one for the power supply, and one for the switch
- two sensor cicruit boards, made below
- one main circuit board, made below
- 3D printed parts, made below
- laster cut wood parts, made below

## Please See

**Please see the following additional pages**

- **[Electronics](electronics.md)** - schematics and circuit boards for the box
- **[Wood](wood.md)** - assembling the wooden box
- **[Top](top.md)** - adding the LEDs, sensors, and switch to the top of the box
- **[Bottom](bottom.md)** - buildling the bottom of the box (Arduino, Batteries, Power Switch, Servos)
- **[Software](software.md)** - a description of the Arduino sketch for the box


## License

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License Version 3 as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Please see [LICENSE.TXT](https://github.com/phorton1/Arduino-libraries-myIOT/blob/master/LICENSE.TXT) for more information.


**Next:** The [**Electronics**](electronics.md), schematics, and circuit boards used in the box ...
