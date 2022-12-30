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
L Device:C C1
U 1 1 6167CA95
P 4100 2000
F 0 "C1" V 3950 1800 50  0000 C CNN
F 1 "100nf" V 4050 1800 50  0000 C CNN
F 2 "cnc3018-PCB:my2pin" H 4138 1850 50  0001 C CNN
F 3 "~" H 4100 2000 50  0001 C CNN
	1    4100 2000
	0    1    1    0   
$EndComp
$Comp
L Device:R R1
U 1 1 6161C310
P 3850 2300
F 0 "R1" V 3950 2250 50  0000 L CNN
F 1 "10K" V 3850 2300 50  0000 C CNN
F 2 "cnc3018-PCB:myResistor" V 3780 2300 50  0001 C CNN
F 3 "~" H 3850 2300 50  0001 C CNN
	1    3850 2300
	0    -1   -1   0   
$EndComp
$Comp
L Device:D_Photo D1
U 1 1 6161B5B7
P 4300 2300
F 0 "D1" H 4550 2200 50  0000 C CNN
F 1 "IR_Detect" H 4300 2200 50  0000 C CNN
F 2 "cnc3018-PCB:my2pin" H 4250 2300 50  0001 C CNN
F 3 "~" H 4250 2300 50  0001 C CNN
	1    4300 2300
	-1   0    0    1   
$EndComp
Wire Wire Line
	3700 2700 3600 2700
Wire Wire Line
	4000 2700 4200 2700
Wire Wire Line
	3600 2300 3700 2300
$Comp
L Device:R R2
U 1 1 6161C011
P 3850 2700
F 0 "R2" V 3750 2650 50  0000 L CNN
F 1 "100" V 3850 2700 50  0000 C CNN
F 2 "cnc3018-PCB:myResistor" V 3780 2700 50  0001 C CNN
F 3 "~" H 3850 2700 50  0001 C CNN
	1    3850 2700
	0    1    1    0   
$EndComp
$Comp
L Device:LED D2
U 1 1 6161AFC5
P 4350 2700
F 0 "D2" H 4050 2800 50  0000 C CNN
F 1 "IR Emit" H 4300 2800 50  0000 C CNN
F 2 "cnc3018-PCB:my2pin" H 4350 2700 50  0001 C CNN
F 3 "~" H 4350 2700 50  0001 C CNN
	1    4350 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 2300 4100 2300
$Comp
L Connector_Generic:Conn_01x04 J1
U 1 1 632B96FA
P 4050 1250
F 0 "J1" V 4200 1500 50  0000 R CNN
F 1 "G 5 L D" V 4200 1350 50  0000 R CNN
F 2 "cnc3018-PCB:my4pin" H 4050 1250 50  0001 C CNN
F 3 "~" H 4050 1250 50  0001 C CNN
	1    4050 1250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3600 2000 3600 1750
Wire Wire Line
	3600 1750 3950 1750
Connection ~ 3600 2000
Wire Wire Line
	3600 2000 3950 2000
Wire Wire Line
	4500 2000 4500 2300
Wire Wire Line
	4500 2000 4500 1750
Wire Wire Line
	4500 1750 4050 1750
Connection ~ 4500 2000
Wire Wire Line
	4100 2300 4100 2500
Wire Wire Line
	4100 2500 4600 2500
Connection ~ 4100 2300
Wire Wire Line
	4100 2300 4200 2300
Connection ~ 3600 2300
Wire Wire Line
	4600 1650 4600 2500
Wire Wire Line
	3600 2300 3600 2700
Wire Wire Line
	3600 2000 3600 2300
Wire Wire Line
	4500 2700 4700 2700
Wire Wire Line
	4700 2700 4700 1550
Wire Wire Line
	4050 1450 4050 1750
Wire Wire Line
	3950 1450 3950 1750
Wire Wire Line
	4150 1550 4150 1450
Wire Wire Line
	4150 1550 4700 1550
Wire Wire Line
	4250 1450 4250 1650
Wire Wire Line
	4250 1650 4600 1650
Wire Wire Line
	4250 2000 4500 2000
Text Notes 2500 3750 0    59   ~ 12
The long lead is the +Anode, and the short lead is the -Cathode.\n\nThe Clear 940 nm emitter is wired in the normal fashion.\n\nThe Black 940 nm receiver is wired backwards, with the cathode connected\nto +5V, and the anode (sensor pin) connected to ground through a 10k resistor.\n
$EndSCHEMATC
