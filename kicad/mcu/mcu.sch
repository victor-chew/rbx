EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "RBX (Robotic Brick eXtension)"
Date "2021-03-17"
Rev "1.1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:C_Small C1
U 1 1 5E283532
P 3150 2800
F 0 "C1" H 3242 2846 50  0000 L CNN
F 1 "1uF" H 3242 2755 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 3150 2800 50  0001 C CNN
F 3 "~" H 3150 2800 50  0001 C CNN
	1    3150 2800
	1    0    0    -1  
$EndComp
Connection ~ 3150 2650
Wire Wire Line
	2550 2750 2550 2950
Wire Wire Line
	3150 2700 3150 2650
Wire Wire Line
	3150 2900 3150 2950
Connection ~ 3150 2950
$Comp
L Connector_Generic:Conn_01x02 J3
U 1 1 5E406BB2
P 2350 2750
F 0 "J3" H 2500 2750 50  0000 C CNN
F 1 "PWR" H 2500 2650 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 2350 2750 50  0001 C CNN
F 3 "~" H 2350 2750 50  0001 C CNN
	1    2350 2750
	-1   0    0    1   
$EndComp
$Comp
L Device:C_Small C2
U 1 1 5E284DCA
P 4200 2800
F 0 "C2" H 4292 2846 50  0000 L CNN
F 1 "2.2uF" H 4250 2700 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 4200 2800 50  0001 C CNN
F 3 "~" H 4200 2800 50  0001 C CNN
	1    4200 2800
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x19_Female J2
U 1 1 5E275440
P 6200 3550
F 0 "J2" H 6250 3550 50  0000 L CNN
F 1 "ESP32-WROOM-32D (R)" V 6550 3150 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x19_P2.54mm_Vertical" H 6200 3550 50  0001 C CNN
F 3 "~" H 6200 3550 50  0001 C CNN
	1    6200 3550
	-1   0    0    -1  
$EndComp
Text Label 6000 2650 0    50   ~ 0
GND
Text Label 6000 2850 0    50   ~ 0
IO22
Text Label 6000 3150 0    50   ~ 0
IO21
Text Label 6000 2750 0    50   ~ 0
IO23
$Comp
L HT7333:HT7333-A L1
U 1 1 5E66254B
P 3700 2650
F 0 "L1" H 3750 3047 60  0000 C CNN
F 1 "HT7833" H 3750 2941 60  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-92" H 3700 2650 60  0001 C CNN
F 3 "" H 3700 2650 60  0000 C CNN
	1    3700 2650
	1    0    0    -1  
$EndComp
Connection ~ 3300 2650
Wire Wire Line
	3300 2650 3400 2650
Connection ~ 3850 2950
Wire Wire Line
	3150 2950 3850 2950
Wire Wire Line
	3150 2650 3300 2650
Wire Wire Line
	4200 2900 4200 2950
Text Label 6000 3450 0    50   ~ 0
IO18
Text Label 6000 3350 0    50   ~ 0
IO19
Text Label 6000 3750 0    50   ~ 0
IO16
Text Label 6000 3650 0    50   ~ 0
IO17
$Comp
L Device:CP C3
U 1 1 5F128BCA
P 2750 2800
F 0 "C3" H 2700 3200 50  0000 L CNN
F 1 "1000uF" H 2650 3050 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D4.0mm_P2.00mm" H 2788 2650 50  0001 C CNN
F 3 "~" H 2750 2800 50  0001 C CNN
	1    2750 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 2650 3150 2650
Wire Wire Line
	2550 2650 2750 2650
Connection ~ 2750 2650
Wire Wire Line
	2550 2950 2750 2950
Connection ~ 2750 2950
$Comp
L Mechanical:MountingHole_Pad H2
U 1 1 5F15A4C5
P 2350 3850
F 0 "H2" H 2450 3899 50  0000 L CNN
F 1 "MountingHole_Pad" H 2450 3808 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.5mm_Pad" H 2350 3850 50  0001 C CNN
F 3 "~" H 2350 3850 50  0001 C CNN
	1    2350 3850
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H1
U 1 1 5F15A6A7
P 2350 3300
F 0 "H1" H 2450 3349 50  0000 L CNN
F 1 "MountingHole_Pad" H 2450 3258 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.5mm_Pad" H 2350 3300 50  0001 C CNN
F 3 "~" H 2350 3300 50  0001 C CNN
	1    2350 3300
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H3
U 1 1 5F174D6B
P 3250 3300
F 0 "H3" H 3350 3349 50  0000 L CNN
F 1 "MountingHole_Pad" H 3350 3258 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.5mm_Pad" H 3250 3300 50  0001 C CNN
F 3 "~" H 3250 3300 50  0001 C CNN
	1    3250 3300
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H4
U 1 1 5F175346
P 3250 3850
F 0 "H4" H 3350 3899 50  0000 L CNN
F 1 "MountingHole_Pad" H 3350 3808 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.5mm_Pad" H 3250 3850 50  0001 C CNN
F 3 "~" H 3250 3850 50  0001 C CNN
	1    3250 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 2950 3150 2950
$Comp
L Device:R R1
U 1 1 5F398C05
P 7300 2700
F 0 "R1" H 7350 2550 50  0000 L CNN
F 1 "4.7K" H 7350 2450 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 7230 2700 50  0001 C CNN
F 3 "~" H 7300 2700 50  0001 C CNN
	1    7300 2700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5F398E5A
P 7550 2700
F 0 "R2" H 7600 2550 50  0000 L CNN
F 1 "4.7k" H 7600 2450 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 7480 2700 50  0001 C CNN
F 3 "~" H 7550 2700 50  0001 C CNN
	1    7550 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 2550 7550 2500
Wire Wire Line
	3850 2950 4200 2950
Connection ~ 4200 2950
Wire Wire Line
	4200 2500 4200 2700
$Comp
L Connector:Conn_01x19_Female J4
U 1 1 5F38A913
P 4500 3550
F 0 "J4" H 4550 3550 50  0000 L CNN
F 1 "(L)" V 4400 2500 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x19_P2.54mm_Vertical" H 4500 3550 50  0001 C CNN
F 3 "~" H 4500 3550 50  0001 C CNN
	1    4500 3550
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x19_Female J1
U 1 1 5E272FE1
P 5200 3550
F 0 "J1" H 5250 3550 50  0000 L CNN
F 1 "ESP32-WROOM-32D (L)" V 5550 3150 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x19_P2.54mm_Vertical" H 5200 3550 50  0001 C CNN
F 3 "~" H 5200 3550 50  0001 C CNN
	1    5200 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 2750 5000 2750
Wire Wire Line
	4700 2850 5000 2850
Wire Wire Line
	4700 2950 5000 2950
Wire Wire Line
	5000 3050 4700 3050
Wire Wire Line
	4700 3150 5000 3150
Wire Wire Line
	5000 3250 4700 3250
Wire Wire Line
	4700 3350 5000 3350
Wire Wire Line
	5000 3450 4700 3450
Wire Wire Line
	5000 3550 4700 3550
Wire Wire Line
	4700 3650 5000 3650
Wire Wire Line
	5000 3750 4700 3750
Wire Wire Line
	4700 3850 5000 3850
Wire Wire Line
	5000 3950 4700 3950
Wire Wire Line
	4700 4050 5000 4050
Wire Wire Line
	5000 4150 4700 4150
Wire Wire Line
	4700 4250 5000 4250
Wire Wire Line
	5000 4350 4700 4350
Wire Wire Line
	4700 4450 5000 4450
Connection ~ 4200 2500
Text Label 5250 3350 0    50   ~ 0
IO33
Text Label 5250 3450 0    50   ~ 0
IO25
Text Label 5250 3750 0    50   ~ 0
IO14
Text Label 5250 3650 0    50   ~ 0
IO27
Text Label 5250 3550 0    50   ~ 0
IO26
Text Label 5250 4050 0    50   ~ 0
IO13
Text Label 5250 3250 0    50   ~ 0
IO32
Text Label 5250 3050 0    50   ~ 0
IO34
Text Label 5250 2850 0    50   ~ 0
IO36
Text Label 5250 2950 0    50   ~ 0
IO39
Text Label 5250 2650 0    50   ~ 0
3V3
Wire Wire Line
	5950 2850 6350 2850
Wire Wire Line
	6800 2750 6400 2750
Wire Wire Line
	6400 2950 6800 2950
Wire Wire Line
	6400 3050 6800 3050
Wire Wire Line
	6400 3250 6800 3250
Wire Wire Line
	6400 3350 6800 3350
Wire Wire Line
	6400 3450 6800 3450
Wire Wire Line
	6400 3550 6800 3550
Wire Wire Line
	6400 3650 6800 3650
Wire Wire Line
	6400 3750 6800 3750
Wire Wire Line
	6800 3850 6400 3850
Wire Wire Line
	6400 4050 6800 4050
Wire Wire Line
	6400 3950 6800 3950
Wire Wire Line
	6800 4150 6400 4150
Wire Wire Line
	6400 4250 6800 4250
Wire Wire Line
	6800 4350 6400 4350
Wire Wire Line
	6400 4450 6800 4450
Wire Wire Line
	7300 2550 7300 2500
Connection ~ 7300 2500
Wire Wire Line
	7300 2500 7550 2500
Wire Wire Line
	5700 2650 5700 4700
Wire Wire Line
	5700 4700 4200 4700
Wire Wire Line
	4200 2950 4200 4700
Wire Wire Line
	4200 2500 4850 2500
Wire Wire Line
	4850 2650 4850 2500
Connection ~ 4850 2650
Wire Wire Line
	4850 2650 5000 2650
Connection ~ 4850 2500
Wire Wire Line
	4850 2500 7300 2500
Wire Wire Line
	7550 3150 7550 2850
$Comp
L Connector:Conn_01x19_Female J5
U 1 1 5F3A4DFD
P 7000 3550
F 0 "J5" H 7050 3550 50  0000 L CNN
F 1 "(R)" V 6900 2500 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x19_P2.54mm_Vertical" H 7000 3550 50  0001 C CNN
F 3 "~" H 7000 3550 50  0001 C CNN
	1    7000 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 3150 6800 3150
Connection ~ 6800 3150
Wire Wire Line
	6800 3150 7550 3150
Wire Wire Line
	6400 2850 6800 2850
Connection ~ 6800 2850
Wire Wire Line
	6800 2850 7300 2850
Wire Wire Line
	5700 2650 6400 2650
Connection ~ 6400 2650
Wire Wire Line
	6400 2650 6800 2650
Wire Wire Line
	4600 2650 4700 2650
Connection ~ 4700 2650
Wire Wire Line
	4700 2650 4850 2650
$EndSCHEMATC
