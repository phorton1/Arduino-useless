EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L cnc3018_Library:ARDUINO_NANO U1
U 1 1 632B0DEA
P 4700 3450
F 0 "U1" H 5528 3496 50  0000 L CNN
F 1 "ARDUINO_NANO" H 5528 3405 50  0000 L CNN
F 2 "cnc3018-PCB:ARDUINO_NANO" H 4350 3450 50  0001 C CNN
F 3 "" H 4350 3450 50  0001 C CNN
	1    4700 3450
	1    0    0    -1  
$EndComp
Text GLabel 5050 3050 1    50   Input ~ 0
L_IR
Text GLabel 4850 3050 1    50   Input ~ 0
R_IR
Text GLabel 5150 3050 1    50   Input ~ 0
U_IR
Text GLabel 4550 3050 1    50   Input ~ 0
L_SENS
Text GLabel 4950 3050 1    50   Input ~ 0
R_SENS
Text GLabel 5450 3050 1    50   Input ~ 0
U_SENS
Text GLabel 4650 3850 3    50   Input ~ 0
WHEEL_1
Text GLabel 4750 3850 3    50   Input ~ 0
WHEEL_2
Text GLabel 4850 3850 3    50   Input ~ 0
WHEEL_3
Text GLabel 4950 3850 3    50   Input ~ 0
WHEEL_4
Text GLabel 4450 3850 3    50   Input ~ 0
ARM
Text GLabel 4550 3850 3    50   Input ~ 0
LID
Text GLabel 5250 3850 3    50   Input ~ 0
LED
Text GLabel 5350 3850 3    50   Input ~ 0
BUT_1
Text GLabel 5450 3850 3    50   Input ~ 0
BUT_2
Text GLabel 5050 3850 3    50   Input ~ 0
FLAG
Text GLabel 4650 3050 1    50   Input ~ 0
SCL
Text GLabel 4750 3050 1    50   Input ~ 0
SDA
$Comp
L power:+3V3 #PWR017
U 1 1 632C2B37
P 5350 3050
F 0 "#PWR017" H 5350 2900 50  0001 C CNN
F 1 "+3V3" V 5350 3250 50  0000 C CNN
F 2 "" H 5350 3050 50  0001 C CNN
F 3 "" H 5350 3050 50  0001 C CNN
	1    5350 3050
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR016
U 1 1 632C4ADE
P 4350 3050
F 0 "#PWR016" H 4350 2900 50  0001 C CNN
F 1 "+5V" V 4350 3250 50  0000 C CNN
F 2 "" H 4350 3050 50  0001 C CNN
F 3 "" H 4350 3050 50  0001 C CNN
	1    4350 3050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR015
U 1 1 632C66C6
P 4150 3050
F 0 "#PWR015" H 4150 2800 50  0001 C CNN
F 1 "GND" V 4150 2850 50  0000 C CNN
F 2 "" H 4150 3050 50  0001 C CNN
F 3 "" H 4150 3050 50  0001 C CNN
	1    4150 3050
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR020
U 1 1 632C8E73
P 4350 3850
F 0 "#PWR020" H 4350 3600 50  0001 C CNN
F 1 "GND" V 4350 3650 50  0000 C CNN
F 2 "" H 4350 3850 50  0001 C CNN
F 3 "" H 4350 3850 50  0001 C CNN
	1    4350 3850
	1    0    0    -1  
$EndComp
Text GLabel 4050 3050 1    50   Input ~ 0
VIN
$Comp
L Connector_Generic:Conn_01x02 J1
U 1 1 632C97C1
P 1400 1050
F 0 "J1" H 1550 900 50  0000 C CNN
F 1 "VIN" H 1550 1000 50  0000 C CNN
F 2 "cnc3018-PCB:my2pin" H 1400 1050 50  0001 C CNN
F 3 "~" H 1400 1050 50  0001 C CNN
	1    1400 1050
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR01
U 1 1 632CB61B
P 1600 1050
F 0 "#PWR01" H 1600 800 50  0001 C CNN
F 1 "GND" V 1600 850 50  0000 C CNN
F 2 "" H 1600 1050 50  0001 C CNN
F 3 "" H 1600 1050 50  0001 C CNN
	1    1600 1050
	0    -1   -1   0   
$EndComp
Text GLabel 1600 950  2    50   Input ~ 0
VIN
$Comp
L Connector_Generic:Conn_01x03 J5
U 1 1 632CB83D
P 1400 1700
F 0 "J5" H 1550 1600 50  0000 C CNN
F 1 "ARM" H 1650 1700 50  0000 R CNN
F 2 "cnc3018-PCB:my3pin" H 1400 1700 50  0001 C CNN
F 3 "~" H 1400 1700 50  0001 C CNN
	1    1400 1700
	-1   0    0    1   
$EndComp
Text GLabel 1600 1700 2    50   Input ~ 0
VIN
$Comp
L power:GND #PWR08
U 1 1 632CED7F
P 1600 1600
F 0 "#PWR08" H 1600 1350 50  0001 C CNN
F 1 "GND" V 1600 1400 50  0000 C CNN
F 2 "" H 1600 1600 50  0001 C CNN
F 3 "" H 1600 1600 50  0001 C CNN
	1    1600 1600
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J6
U 1 1 632D7059
P 1400 2100
F 0 "J6" H 1550 2000 50  0000 C CNN
F 1 "LID" H 1650 2100 50  0000 R CNN
F 2 "cnc3018-PCB:my3pin" H 1400 2100 50  0001 C CNN
F 3 "~" H 1400 2100 50  0001 C CNN
	1    1400 2100
	-1   0    0    1   
$EndComp
Text GLabel 1600 2100 2    50   Input ~ 0
VIN
$Comp
L power:GND #PWR010
U 1 1 632D7060
P 1600 2000
F 0 "#PWR010" H 1600 1750 50  0001 C CNN
F 1 "GND" V 1600 1800 50  0000 C CNN
F 2 "" H 1600 2000 50  0001 C CNN
F 3 "" H 1600 2000 50  0001 C CNN
	1    1600 2000
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J7
U 1 1 632D750C
P 1400 2500
F 0 "J7" H 1550 2400 50  0000 C CNN
F 1 "WHEEL_1" H 1850 2500 50  0000 R CNN
F 2 "cnc3018-PCB:my3pin" H 1400 2500 50  0001 C CNN
F 3 "~" H 1400 2500 50  0001 C CNN
	1    1400 2500
	-1   0    0    1   
$EndComp
Text GLabel 1600 2500 2    50   Input ~ 0
VIN
$Comp
L power:GND #PWR012
U 1 1 632D7513
P 1600 2400
F 0 "#PWR012" H 1600 2150 50  0001 C CNN
F 1 "GND" V 1600 2200 50  0000 C CNN
F 2 "" H 1600 2400 50  0001 C CNN
F 3 "" H 1600 2400 50  0001 C CNN
	1    1600 2400
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J8
U 1 1 632DCC73
P 1400 2900
F 0 "J8" H 1550 2800 50  0000 C CNN
F 1 "WHEEL_2" H 1850 2900 50  0000 R CNN
F 2 "cnc3018-PCB:my3pin" H 1400 2900 50  0001 C CNN
F 3 "~" H 1400 2900 50  0001 C CNN
	1    1400 2900
	-1   0    0    1   
$EndComp
Text GLabel 1600 2900 2    50   Input ~ 0
VIN
$Comp
L power:GND #PWR014
U 1 1 632DCC7A
P 1600 2800
F 0 "#PWR014" H 1600 2550 50  0001 C CNN
F 1 "GND" V 1600 2600 50  0000 C CNN
F 2 "" H 1600 2800 50  0001 C CNN
F 3 "" H 1600 2800 50  0001 C CNN
	1    1600 2800
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J9
U 1 1 632DD10C
P 1400 3300
F 0 "J9" H 1550 3200 50  0000 C CNN
F 1 "WHEEL_3" H 1850 3300 50  0000 R CNN
F 2 "cnc3018-PCB:my3pin" H 1400 3300 50  0001 C CNN
F 3 "~" H 1400 3300 50  0001 C CNN
	1    1400 3300
	-1   0    0    1   
$EndComp
Text GLabel 1600 3300 2    50   Input ~ 0
VIN
$Comp
L power:GND #PWR018
U 1 1 632DD113
P 1600 3200
F 0 "#PWR018" H 1600 2950 50  0001 C CNN
F 1 "GND" V 1600 3000 50  0000 C CNN
F 2 "" H 1600 3200 50  0001 C CNN
F 3 "" H 1600 3200 50  0001 C CNN
	1    1600 3200
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J10
U 1 1 632DDF40
P 1400 3700
F 0 "J10" H 1550 3600 50  0000 C CNN
F 1 "WHEEL_4" H 1850 3700 50  0000 R CNN
F 2 "cnc3018-PCB:my3pin" H 1400 3700 50  0001 C CNN
F 3 "~" H 1400 3700 50  0001 C CNN
	1    1400 3700
	-1   0    0    1   
$EndComp
Text GLabel 1600 3700 2    50   Input ~ 0
VIN
$Comp
L power:GND #PWR019
U 1 1 632DDF47
P 1600 3600
F 0 "#PWR019" H 1600 3350 50  0001 C CNN
F 1 "GND" V 1600 3400 50  0000 C CNN
F 2 "" H 1600 3600 50  0001 C CNN
F 3 "" H 1600 3600 50  0001 C CNN
	1    1600 3600
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J11
U 1 1 632DE268
P 1400 4100
F 0 "J11" H 1550 4000 50  0000 C CNN
F 1 "FLAG" H 1700 4100 50  0000 R CNN
F 2 "cnc3018-PCB:my3pin" H 1400 4100 50  0001 C CNN
F 3 "~" H 1400 4100 50  0001 C CNN
	1    1400 4100
	-1   0    0    1   
$EndComp
Text GLabel 1600 4100 2    50   Input ~ 0
VIN
$Comp
L power:GND #PWR021
U 1 1 632DE26F
P 1600 4000
F 0 "#PWR021" H 1600 3750 50  0001 C CNN
F 1 "GND" V 1600 3800 50  0000 C CNN
F 2 "" H 1600 4000 50  0001 C CNN
F 3 "" H 1600 4000 50  0001 C CNN
	1    1600 4000
	0    -1   -1   0   
$EndComp
Text GLabel 1600 2600 2    50   Input ~ 0
WHEEL_1
Text GLabel 1600 3000 2    50   Input ~ 0
WHEEL_2
Text GLabel 1600 3400 2    50   Input ~ 0
WHEEL_3
Text GLabel 1600 3800 2    50   Input ~ 0
WHEEL_4
Text GLabel 1600 1800 2    50   Input ~ 0
ARM
Text GLabel 1600 2200 2    50   Input ~ 0
LID
Text GLabel 1600 4200 2    50   Input ~ 0
FLAG
$Comp
L Connector_Generic:Conn_01x04 J2
U 1 1 632F7A69
P 3500 1350
F 0 "J2" V 3700 1350 50  0000 R CNN
F 1 "LEFT" V 3600 1300 50  0000 C CNN
F 2 "cnc3018-PCB:my4pin" H 3500 1350 50  0001 C CNN
F 3 "~" H 3500 1350 50  0001 C CNN
	1    3500 1350
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR02
U 1 1 632FA8A1
P 3400 1550
F 0 "#PWR02" H 3400 1300 50  0001 C CNN
F 1 "GND" V 3400 1350 50  0000 C CNN
F 2 "" H 3400 1550 50  0001 C CNN
F 3 "" H 3400 1550 50  0001 C CNN
	1    3400 1550
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR03
U 1 1 632FD66D
P 3500 1550
F 0 "#PWR03" H 3500 1400 50  0001 C CNN
F 1 "+5V" V 3500 1750 50  0000 C CNN
F 2 "" H 3500 1550 50  0001 C CNN
F 3 "" H 3500 1550 50  0001 C CNN
	1    3500 1550
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J3
U 1 1 632FFE72
P 4100 1350
F 0 "J3" V 4300 1350 50  0000 R CNN
F 1 "RIGHT" V 4200 1300 50  0000 C CNN
F 2 "cnc3018-PCB:my4pin" H 4100 1350 50  0001 C CNN
F 3 "~" H 4100 1350 50  0001 C CNN
	1    4100 1350
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR04
U 1 1 632FFE78
P 4000 1550
F 0 "#PWR04" H 4000 1300 50  0001 C CNN
F 1 "GND" V 4000 1350 50  0000 C CNN
F 2 "" H 4000 1550 50  0001 C CNN
F 3 "" H 4000 1550 50  0001 C CNN
	1    4000 1550
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR05
U 1 1 632FFE7E
P 4100 1550
F 0 "#PWR05" H 4100 1400 50  0001 C CNN
F 1 "+5V" V 4100 1750 50  0000 C CNN
F 2 "" H 4100 1550 50  0001 C CNN
F 3 "" H 4100 1550 50  0001 C CNN
	1    4100 1550
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J4
U 1 1 63300773
P 4650 1350
F 0 "J4" V 4850 1350 50  0000 R CNN
F 1 "UNUSED" V 4750 1300 50  0000 C CNN
F 2 "cnc3018-PCB:my4pin" H 4650 1350 50  0001 C CNN
F 3 "~" H 4650 1350 50  0001 C CNN
	1    4650 1350
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR06
U 1 1 63300779
P 4550 1550
F 0 "#PWR06" H 4550 1300 50  0001 C CNN
F 1 "GND" V 4550 1350 50  0000 C CNN
F 2 "" H 4550 1550 50  0001 C CNN
F 3 "" H 4550 1550 50  0001 C CNN
	1    4550 1550
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR07
U 1 1 6330077F
P 4650 1550
F 0 "#PWR07" H 4650 1400 50  0001 C CNN
F 1 "+5V" V 4650 1750 50  0000 C CNN
F 2 "" H 4650 1550 50  0001 C CNN
F 3 "" H 4650 1550 50  0001 C CNN
	1    4650 1550
	-1   0    0    1   
$EndComp
Text GLabel 3700 1550 3    50   Input ~ 0
L_SENS
Text GLabel 4850 1550 3    50   Input ~ 0
U_SENS
Text GLabel 4300 1550 3    50   Input ~ 0
R_SENS
Text GLabel 3600 1550 3    50   Input ~ 0
L_IR
Text GLabel 4750 1550 3    50   Input ~ 0
U_IR
Text GLabel 4200 1550 3    50   Input ~ 0
R_IR
$Comp
L Connector_Generic:Conn_01x03 J12
U 1 1 63327EF8
P 1350 4800
F 0 "J12" H 1500 4700 50  0000 C CNN
F 1 "LED" H 1650 4800 50  0000 R CNN
F 2 "cnc3018-PCB:my3pin" H 1350 4800 50  0001 C CNN
F 3 "~" H 1350 4800 50  0001 C CNN
	1    1350 4800
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR022
U 1 1 63327EFF
P 1550 4700
F 0 "#PWR022" H 1550 4450 50  0001 C CNN
F 1 "GND" V 1550 4500 50  0000 C CNN
F 2 "" H 1550 4700 50  0001 C CNN
F 3 "" H 1550 4700 50  0001 C CNN
	1    1550 4700
	0    -1   -1   0   
$EndComp
$Comp
L power:+5V #PWR023
U 1 1 6332BC14
P 1550 4900
F 0 "#PWR023" H 1550 4750 50  0001 C CNN
F 1 "+5V" V 1550 5100 50  0000 C CNN
F 2 "" H 1550 4900 50  0001 C CNN
F 3 "" H 1550 4900 50  0001 C CNN
	1    1550 4900
	0    1    1    0   
$EndComp
Text GLabel 1550 4800 2    50   Input ~ 0
LED
$Comp
L Connector_Generic:Conn_01x04 J13
U 1 1 6332FD8A
P 3600 5900
F 0 "J13" V 3800 5900 50  0000 R CNN
F 1 "COMPASS" V 3700 5850 50  0000 C CNN
F 2 "cnc3018-PCB:my4pin" H 3600 5900 50  0001 C CNN
F 3 "~" H 3600 5900 50  0001 C CNN
	1    3600 5900
	0    1    1    0   
$EndComp
$Comp
L power:+3V3 #PWR024
U 1 1 633314BC
P 3400 5700
F 0 "#PWR024" H 3400 5550 50  0001 C CNN
F 1 "+3V3" V 3400 5900 50  0000 C CNN
F 2 "" H 3400 5700 50  0001 C CNN
F 3 "" H 3400 5700 50  0001 C CNN
	1    3400 5700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR025
U 1 1 633324CD
P 3500 5700
F 0 "#PWR025" H 3500 5450 50  0001 C CNN
F 1 "GND" V 3500 5500 50  0000 C CNN
F 2 "" H 3500 5700 50  0001 C CNN
F 3 "" H 3500 5700 50  0001 C CNN
	1    3500 5700
	-1   0    0    1   
$EndComp
Text GLabel 3600 5700 1    50   Input ~ 0
SCL
Text GLabel 3700 5700 1    50   Input ~ 0
SDA
$Comp
L Switch:SW_SPST SW1
U 1 1 6333D989
P 6850 1700
F 0 "SW1" H 6850 1935 50  0000 C CNN
F 1 "BUTTON_1" H 6850 1844 50  0000 C CNN
F 2 "cnc3018-PCB:right_angle_button" H 6850 1700 50  0001 C CNN
F 3 "~" H 6850 1700 50  0001 C CNN
	1    6850 1700
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW2
U 1 1 6333F07E
P 6850 2150
F 0 "SW2" H 6850 2385 50  0000 C CNN
F 1 "BUTTON_2" H 6850 2294 50  0000 C CNN
F 2 "cnc3018-PCB:right_angle_button" H 6850 2150 50  0001 C CNN
F 3 "~" H 6850 2150 50  0001 C CNN
	1    6850 2150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR09
U 1 1 63344CDC
P 6650 1700
F 0 "#PWR09" H 6650 1450 50  0001 C CNN
F 1 "GND" V 6650 1500 50  0000 C CNN
F 2 "" H 6650 1700 50  0001 C CNN
F 3 "" H 6650 1700 50  0001 C CNN
	1    6650 1700
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR011
U 1 1 63344E7E
P 7050 2150
F 0 "#PWR011" H 7050 1900 50  0001 C CNN
F 1 "GND" V 7050 1950 50  0000 C CNN
F 2 "" H 7050 2150 50  0001 C CNN
F 3 "" H 7050 2150 50  0001 C CNN
	1    7050 2150
	0    -1   -1   0   
$EndComp
Text GLabel 7050 1700 2    50   Input ~ 0
BUT_1
Text GLabel 6650 2150 0    50   Input ~ 0
BUT_2
$Comp
L Connector_Generic:Conn_01x02 J14
U 1 1 63387135
P 1350 5350
F 0 "J14" H 1500 5200 50  0000 C CNN
F 1 "SWITCH" H 1550 5300 50  0000 C CNN
F 2 "cnc3018-PCB:my2pin" H 1350 5350 50  0001 C CNN
F 3 "~" H 1350 5350 50  0001 C CNN
	1    1350 5350
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 6338713B
P 1550 5350
F 0 "#PWR0101" H 1550 5100 50  0001 C CNN
F 1 "GND" V 1550 5150 50  0000 C CNN
F 2 "" H 1550 5350 50  0001 C CNN
F 3 "" H 1550 5350 50  0001 C CNN
	1    1550 5350
	0    -1   -1   0   
$EndComp
Text GLabel 1550 5250 2    50   Input ~ 0
SWITCH
Text GLabel 5150 3850 3    50   Input ~ 0
SWITCH
Text Notes 7800 7000 0    50   ~ 0
Note: this is the correct schematic for the "fixed" V1 board.\nIf I make another boartd, I should rework pin assignements\nA6 and A7 CANNOT be outputs (for IR Leds) ... 
$EndSCHEMATC
