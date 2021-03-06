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
L Connector:Conn_01x02_Male J5
U 1 1 5E35B4E7
P 3500 3650
F 0 "J5" H 3400 3650 50  0000 C CNN
F 1 "PORT-2B" H 3250 3550 50  0000 C CNN
F 2 "Connector_JST:JST_XH_B2B-XH-A_1x02_P2.50mm_Vertical" H 3500 3650 50  0001 C CNN
F 3 "~" H 3500 3650 50  0001 C CNN
	1    3500 3650
	1    0    0    1   
$EndComp
$Comp
L Connector:Conn_01x02_Male J6
U 1 1 5E35B82A
P 3500 3950
F 0 "J6" H 3400 3950 50  0000 C CNN
F 1 "PORT-2C" H 3250 3850 50  0000 C CNN
F 2 "Connector_JST:JST_XH_B2B-XH-A_1x02_P2.50mm_Vertical" H 3500 3950 50  0001 C CNN
F 3 "~" H 3500 3950 50  0001 C CNN
	1    3500 3950
	1    0    0    1   
$EndComp
$Comp
L Connector:Conn_01x04_Male J11
U 1 1 5E37692A
P 7550 2850
F 0 "J11" H 7522 2732 50  0000 R CNN
F 1 "PORT-4A" H 7522 2823 50  0000 R CNN
F 2 "Connector_JST:JST_XH_B4B-XH-A_1x04_P2.50mm_Vertical" H 7550 2850 50  0001 C CNN
F 3 "~" H 7550 2850 50  0001 C CNN
	1    7550 2850
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x04_Male J12
U 1 1 5E37F2C7
P 7550 3450
F 0 "J12" H 7522 3332 50  0000 R CNN
F 1 "PORT-4B" H 7522 3423 50  0000 R CNN
F 2 "Connector_JST:JST_XH_B4B-XH-A_1x04_P2.50mm_Vertical" H 7550 3450 50  0001 C CNN
F 3 "~" H 7550 3450 50  0001 C CNN
	1    7550 3450
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x04_Male J13
U 1 1 5E3864A9
P 7550 4050
F 0 "J13" H 7522 3932 50  0000 R CNN
F 1 "PORT-4C" H 7522 4023 50  0000 R CNN
F 2 "Connector_JST:JST_XH_B4B-XH-A_1x04_P2.50mm_Vertical" H 7550 4050 50  0001 C CNN
F 3 "~" H 7550 4050 50  0001 C CNN
	1    7550 4050
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x04_Male J14
U 1 1 5E386AB4
P 7550 4700
F 0 "J14" H 7522 4582 50  0000 R CNN
F 1 "PORT-4D" H 7522 4673 50  0000 R CNN
F 2 "Connector_JST:JST_XH_B4B-XH-A_1x04_P2.50mm_Vertical" H 7550 4700 50  0001 C CNN
F 3 "~" H 7550 4700 50  0001 C CNN
	1    7550 4700
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x04_Male J15
U 1 1 5E38E513
P 7550 5350
F 0 "J15" H 7522 5232 50  0000 R CNN
F 1 "PORT-4E" H 7522 5323 50  0000 R CNN
F 2 "Connector_JST:JST_XH_B4B-XH-A_1x04_P2.50mm_Vertical" H 7550 5350 50  0001 C CNN
F 3 "~" H 7550 5350 50  0001 C CNN
	1    7550 5350
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x03_Male J7
U 1 1 5E365EED
P 3500 4300
F 0 "J7" H 3473 4232 50  0000 R CNN
F 1 "PORT-3A" H 3473 4323 50  0000 R CNN
F 2 "Connector_JST:JST_XH_B3B-XH-A_1x03_P2.50mm_Vertical" H 3500 4300 50  0001 C CNN
F 3 "~" H 3500 4300 50  0001 C CNN
	1    3500 4300
	1    0    0    1   
$EndComp
Text Label 4800 2950 0    50   ~ 0
IO39
Text Label 4800 2850 0    50   ~ 0
IO36
Text Label 4800 3050 0    50   ~ 0
IO34
Wire Wire Line
	3700 3650 4200 3650
Wire Wire Line
	3700 3300 4200 3300
$Comp
L Connector:Conn_01x19_Female J1
U 1 1 5E272FE1
P 5200 3550
F 0 "J1" H 5250 3550 50  0000 L CNN
F 1 "ESP32-WROOM-32D (L)" V 5400 3100 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x19_P2.54mm_Vertical" H 5200 3550 50  0001 C CNN
F 3 "~" H 5200 3550 50  0001 C CNN
	1    5200 3550
	1    0    0    -1  
$EndComp
Text Label 4800 3250 0    50   ~ 0
IO32
$Comp
L Connector:Conn_01x03_Male J8
U 1 1 5E36E5BE
P 3500 4700
F 0 "J8" H 3472 4632 50  0000 R CNN
F 1 "PORT-3B" H 3472 4723 50  0000 R CNN
F 2 "Connector_JST:JST_XH_B3B-XH-A_1x03_P2.50mm_Vertical" H 3500 4700 50  0001 C CNN
F 3 "~" H 3500 4700 50  0001 C CNN
	1    3500 4700
	1    0    0    1   
$EndComp
$Comp
L Connector:Conn_01x03_Male J9
U 1 1 5E372BD6
P 3500 5100
F 0 "J9" H 3472 5032 50  0000 R CNN
F 1 "PORT-3C" H 3472 5123 50  0000 R CNN
F 2 "Connector_JST:JST_XH_B3B-XH-A_1x03_P2.50mm_Vertical" H 3500 5100 50  0001 C CNN
F 3 "~" H 3500 5100 50  0001 C CNN
	1    3500 5100
	1    0    0    1   
$EndComp
$Comp
L Connector:Conn_01x03_Male J10
U 1 1 5E3924EA
P 3500 5500
F 0 "J10" H 3472 5432 50  0000 R CNN
F 1 "PORT-3D" H 3472 5523 50  0000 R CNN
F 2 "Connector_JST:JST_XH_B3B-XH-A_1x03_P2.50mm_Vertical" H 3500 5500 50  0001 C CNN
F 3 "~" H 3500 5500 50  0001 C CNN
	1    3500 5500
	1    0    0    1   
$EndComp
$Comp
L Connector:Conn_01x19_Female J2
U 1 1 5E275440
P 5800 3550
F 0 "J2" H 5850 3550 50  0000 L CNN
F 1 "ESP32-WROOM-32D (R)" V 6000 3100 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x19_P2.54mm_Vertical" H 5800 3550 50  0001 C CNN
F 3 "~" H 5800 3550 50  0001 C CNN
	1    5800 3550
	-1   0    0    -1  
$EndComp
Connection ~ 4200 3650
Wire Wire Line
	4200 3300 4200 3650
Text Label 6050 2650 0    50   ~ 0
GND
Connection ~ 4200 3950
Wire Wire Line
	4200 3650 4200 3950
Wire Wire Line
	3700 3950 4200 3950
Wire Wire Line
	6000 2650 6650 2650
Wire Wire Line
	4600 3050 5000 3050
Wire Wire Line
	4400 2850 5000 2850
Wire Wire Line
	4500 2950 5000 2950
Wire Wire Line
	7350 2950 6650 2950
Wire Wire Line
	6650 2650 6650 2950
Connection ~ 6650 2950
Wire Wire Line
	7350 3550 6650 3550
Wire Wire Line
	6650 2950 6650 3550
Connection ~ 6650 3550
Wire Wire Line
	6650 3550 6650 4150
Wire Wire Line
	7350 4150 6650 4150
Connection ~ 6650 4150
Wire Wire Line
	6650 4150 6650 4800
Wire Wire Line
	7350 4800 6650 4800
Connection ~ 6650 4800
Wire Wire Line
	7350 5450 6650 5450
Wire Wire Line
	6650 5450 6650 4800
Wire Wire Line
	7350 2850 6750 2850
Wire Wire Line
	6750 2850 6750 2500
Wire Wire Line
	6750 2850 6750 3450
Wire Wire Line
	6750 3450 7350 3450
Connection ~ 6750 2850
Wire Wire Line
	6750 3450 6750 4050
Wire Wire Line
	6750 4050 7350 4050
Connection ~ 6750 3450
Wire Wire Line
	6750 4050 6750 4700
Wire Wire Line
	6750 4700 7350 4700
Connection ~ 6750 4050
Wire Wire Line
	6750 5350 7350 5350
Connection ~ 6750 4700
Text Label 6050 2850 0    50   ~ 0
IO22
Text Label 6050 3150 0    50   ~ 0
IO21
Wire Wire Line
	6550 5250 6550 3150
Wire Wire Line
	6550 3150 6000 3150
Wire Wire Line
	6450 5150 6450 2850
Wire Wire Line
	6450 2850 6000 2850
Wire Wire Line
	6850 2750 6850 4550
Wire Wire Line
	4900 4550 4900 4050
Wire Wire Line
	4900 4050 5000 4050
Wire Wire Line
	7350 2750 6850 2750
Wire Wire Line
	7350 2650 6900 2650
Wire Wire Line
	6900 2650 6900 4600
Wire Wire Line
	6900 4600 4850 4600
Wire Wire Line
	4850 4600 4850 3750
Wire Wire Line
	4850 3750 5000 3750
Text Label 6050 2750 0    50   ~ 0
IO23
Wire Wire Line
	7350 4600 7000 4600
Wire Wire Line
	7000 4600 7000 4250
Wire Wire Line
	7000 4250 6350 4250
Wire Wire Line
	6350 4250 6350 2750
Wire Wire Line
	6350 2750 6000 2750
Wire Wire Line
	6850 4550 4900 4550
Wire Wire Line
	5000 3450 4950 3450
Wire Wire Line
	4950 3450 4950 4500
Wire Wire Line
	4950 4500 7350 4500
Wire Wire Line
	4300 2650 5000 2650
Text Label 4800 2650 0    50   ~ 0
3V3
Text Label 4800 4050 0    50   ~ 0
IO13
Wire Wire Line
	4300 2650 4300 2500
Connection ~ 4300 2650
Wire Wire Line
	4300 2500 6750 2500
Wire Wire Line
	4700 3350 5000 3350
Text Label 4800 3550 0    50   ~ 0
IO26
Text Label 4800 3650 0    50   ~ 0
IO27
Text Label 4800 3750 0    50   ~ 0
IO14
Text Label 4800 3450 0    50   ~ 0
IO25
Text Label 4800 3350 0    50   ~ 0
IO33
Wire Wire Line
	3700 3200 4650 3200
Wire Wire Line
	4650 3550 5000 3550
Wire Wire Line
	4550 3550 3700 3550
Wire Wire Line
	4550 3650 5000 3650
Wire Wire Line
	4650 3200 4650 3550
Wire Wire Line
	4550 3550 4550 3650
Wire Wire Line
	4450 3850 4450 3250
Wire Wire Line
	4450 3250 5000 3250
Wire Wire Line
	7150 3850 7150 3600
Wire Wire Line
	7150 3600 6250 3600
Wire Wire Line
	6250 3600 6250 3350
Wire Wire Line
	6250 3350 6000 3350
Wire Wire Line
	7150 3850 7350 3850
Wire Wire Line
	7350 3950 7050 3950
Wire Wire Line
	7050 3950 7050 3650
Wire Wire Line
	7050 3650 6200 3650
Wire Wire Line
	6200 3650 6200 3450
Wire Wire Line
	6200 3450 6000 3450
Text Label 6050 3450 0    50   ~ 0
IO18
Text Label 6050 3350 0    50   ~ 0
IO19
Wire Wire Line
	7350 3350 7000 3350
Wire Wire Line
	7000 3350 7000 3750
Wire Wire Line
	7000 3750 6000 3750
Wire Wire Line
	7350 3250 6950 3250
Wire Wire Line
	6950 3250 6950 3700
Wire Wire Line
	6950 3700 6100 3700
Wire Wire Line
	6100 3700 6100 3650
Wire Wire Line
	6100 3650 6000 3650
Text Label 6050 3750 0    50   ~ 0
IO16
Text Label 6050 3650 0    50   ~ 0
IO17
Wire Wire Line
	4500 5600 3700 5600
Wire Wire Line
	4500 2950 4500 5600
Wire Wire Line
	3700 5500 4200 5500
Wire Wire Line
	3700 5400 4300 5400
Wire Wire Line
	4400 5200 3700 5200
Wire Wire Line
	4400 2850 4400 5200
Wire Wire Line
	3700 5100 4200 5100
Connection ~ 4200 5100
Wire Wire Line
	4200 5100 4200 5500
Wire Wire Line
	3700 5000 4300 5000
Connection ~ 4300 5000
Wire Wire Line
	4300 5000 4300 5400
Wire Wire Line
	4600 4800 3700 4800
Wire Wire Line
	4600 3050 4600 4800
Wire Wire Line
	3700 4600 4300 4600
Connection ~ 4300 4600
Wire Wire Line
	4300 4600 4300 5000
Wire Wire Line
	4200 3950 4200 4300
Wire Wire Line
	4300 2650 4300 4200
Wire Wire Line
	4700 4400 3700 4400
Wire Wire Line
	4700 3350 4700 4400
Wire Wire Line
	3700 4300 4200 4300
Connection ~ 4200 4300
Wire Wire Line
	4200 4300 4200 4700
Wire Wire Line
	3700 4200 4300 4200
Connection ~ 4300 4200
Wire Wire Line
	4300 4200 4300 4600
Wire Wire Line
	6450 5150 7350 5150
Wire Wire Line
	6750 4700 6750 5350
Wire Wire Line
	6550 5250 7350 5250
$Comp
L Connector:Conn_01x02_Male J3
U 1 1 5F3B2D50
P 3500 3300
F 0 "J3" H 3400 3300 50  0000 C CNN
F 1 "PORT-2A" H 3250 3200 50  0000 C CNN
F 2 "Connector_JST:JST_XH_B2B-XH-A_1x02_P2.50mm_Vertical" H 3500 3300 50  0001 C CNN
F 3 "~" H 3500 3300 50  0001 C CNN
	1    3500 3300
	1    0    0    1   
$EndComp
Wire Wire Line
	3700 4700 4200 4700
Connection ~ 4200 4700
Wire Wire Line
	4200 4700 4200 5100
Wire Wire Line
	5600 4700 5600 4800
Wire Wire Line
	5600 4800 6650 4800
Wire Wire Line
	4200 4700 5600 4700
Wire Wire Line
	4450 3850 3700 3850
$EndSCHEMATC
