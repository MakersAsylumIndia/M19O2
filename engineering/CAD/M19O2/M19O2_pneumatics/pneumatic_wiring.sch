EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A3 11693 16535 portrait
encoding utf-8
Sheet 1 1
Title "M19O2 Pneumatic Circuit"
Date "2021-07-05"
Rev "v1"
Comp "Maker's Asylum"
Comment1 "Anool Mahidharia"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L M19O2_pneumatic:Compressor M?
U 1 1 60E262DA
P 1625 14900
F 0 "M?" H 1525 14900 100 0001 L CNB
F 1 "COMPRESSOR" H 1450 15375 75  0000 L CNB
F 2 "" H 1625 14900 50  0001 C CNN
F 3 "" H 1625 14900 50  0001 C CNN
	1    1625 14900
	1    0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:CoolingCoil CC?
U 1 1 60E268B1
P 4550 14275
F 0 "CC?" H 4550 14850 50  0001 C CNN
F 1 "COOLING COILS" H 4550 14775 75  0000 C CNB
F 2 "" H 4550 14275 50  0001 C CNN
F 3 "" H 4550 14275 50  0001 C CNN
	1    4550 14275
	1    0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:WaterSeparator WS?
U 1 1 60E27AD6
P 6400 14275
F 0 "WS?" H 6425 14875 50  0001 C CNN
F 1 "WATER SEPARATOR" H 6400 14775 75  0000 C CNB
F 2 "" H 6400 14275 50  0001 C CNN
F 3 "" H 6400 14275 50  0001 C CNN
	1    6400 14275
	1    0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:PrRegGauge PR?
U 1 1 60E284C0
P 6975 12025
F 0 "PR?" H 7175 13150 50  0001 C CNN
F 1 "HIGH PRESSURE REGULATOR WITH GAUGE (OPTION)" H 6850 13050 75  0000 C CNB
F 2 "" H 6975 12025 50  0001 C CNN
F 3 "" H 6975 12025 50  0001 C CNN
	1    6975 12025
	-1   0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:Valve_5_3_Solenoid V?
U 1 1 60E2A646
P 5850 9400
F 0 "V?" H 4300 9450 50  0001 R CNN
F 1 "VALVE 5/3 24V SOLENOID" H 8875 9700 75  0000 R CNB
F 2 "" H 5850 9400 50  0001 C CNN
F 3 "" H 5850 9400 50  0001 C CNN
	1    5850 9400
	1    0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:Reservoir T?
U 1 1 60E30E69
P 8575 13125
F 0 "T?" H 8875 13250 50  0001 L CNN
F 1 "DESSICANT (OPTION)" H 8875 13100 75  0000 L CNB
F 2 "" H 8575 13125 50  0001 C CNN
F 3 "" H 8575 13125 50  0001 C CNN
	1    8575 13125
	1    0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:Reservoir T?
U 1 1 60E4AB65
P 3350 7900
F 0 "T?" H 3650 8025 50  0001 L CNN
F 1 "LiX MOL SIEVE A" H 3650 7900 75  0000 L CNB
F 2 "" H 3350 7900 50  0001 C CNN
F 3 "" H 3350 7900 50  0001 C CNN
	1    3350 7900
	1    0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:Reservoir T?
U 1 1 60E4D726
P 8350 7900
F 0 "T?" H 8650 8025 50  0001 L CNN
F 1 "LiX MOL SIEVE B" H 7100 7900 75  0000 L CNB
F 2 "" H 8350 7900 50  0001 C CNN
F 3 "" H 8350 7900 50  0001 C CNN
	1    8350 7900
	1    0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:Orifice O?
U 1 1 60E4F664
P 3850 7100
F 0 "O?" H 3850 7475 50  0001 C CNN
F 1 "ORIFICE" H 3850 7375 75  0000 C CNB
F 2 "" H 3850 7100 50  0001 C CNN
F 3 "" H 3850 7100 50  0001 C CNN
	1    3850 7100
	1    0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:Orifice O?
U 1 1 60E5046F
P 7850 7100
F 0 "O?" H 7850 7475 50  0001 C CNN
F 1 "ORIFICE" H 7850 7375 75  0000 C CNB
F 2 "" H 7850 7100 50  0001 C CNN
F 3 "" H 7850 7100 50  0001 C CNN
	1    7850 7100
	1    0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:Valve_non_return U?
U 1 1 60E547FE
P 5850 6200
F 0 "U?" V 5825 5875 50  0001 R CNN
F 1 "VALVE, NON-RETURN" V 5850 5875 75  0000 R CNB
F 2 "" H 5850 6200 50  0001 C CNN
F 3 "" H 5850 6200 50  0001 C CNN
	1    5850 6200
	0    -1   -1   0   
$EndComp
$Comp
L M19O2_pneumatic:Reservoir T?
U 1 1 60E55320
P 5850 4950
F 0 "T?" H 6150 5075 50  0001 L CNN
F 1 "SURGE TANK" H 6150 4925 75  0000 L CNB
F 2 "" H 5850 4950 50  0001 C CNN
F 3 "" H 5850 4950 50  0001 C CNN
	1    5850 4950
	1    0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:PrRegGauge PR?
U 1 1 60E568C7
P 2900 2825
F 0 "PR?" H 3100 3950 50  0001 C CNN
F 1 "LOW PRESSURE REGULATOR" H 2900 2050 75  0000 C CNB
F 2 "" H 2900 2825 50  0001 C CNN
F 3 "" H 2900 2825 50  0001 C CNN
	1    2900 2825
	1    0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:Filter F?
U 1 1 60E5B5E7
P 4625 2825
F 0 "F?" H 4650 3400 50  0001 C CNN
F 1 "FILTER HEPA" H 4625 2050 75  0000 C CNB
F 2 "" H 4625 2825 50  0001 C CNN
F 3 "" H 4625 2825 50  0001 C CNN
	1    4625 2825
	1    0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:Flow_Reg_uni_dir O?
U 1 1 60E5C14C
P 7525 1900
F 0 "O?" H 7575 2600 50  0001 C CNN
F 1 "BLEED VALVE, UNI-DIRECTION" H 7525 2850 75  0000 C CNB
F 2 "" H 7525 1900 50  0001 C CNN
F 3 "" H 7525 1900 50  0001 C CNN
	1    7525 1900
	1    0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:OOM102_flow U?
U 1 1 60E60645
P 6075 1300
F 0 "U?" V 7100 1325 50  0001 C CNN
F 1 "O2 SENSOR" V 5725 575 75  0000 C CNB
F 2 "" H 6075 1300 50  0001 C CNN
F 3 "" H 6075 1300 50  0001 C CNN
	1    6075 1300
	0    1    1    0   
$EndComp
$Comp
L M19O2_pneumatic:flow_meter U?
U 1 1 60E63014
P 6150 2825
F 0 "U?" H 6175 3275 50  0001 C CNN
F 1 "ROTAMETER, 15 LPM" H 6150 2050 75  0000 C CNB
F 2 "" H 6300 2825 50  0001 C CNN
F 3 "" H 6300 2825 50  0001 C CNN
	1    6150 2825
	1    0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:Humidifier H?
U 1 1 60E64521
P 7525 2825
F 0 "H?" H 7500 3125 50  0001 C CNN
F 1 "HUMIDIFIER" H 7525 2050 75  0000 C CNB
F 2 "" H 7525 2825 50  0001 C CNN
F 3 "" H 7525 2825 50  0001 C CNN
	1    7525 2825
	1    0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:pressure_safety_switch_adj U?
U 1 1 60E670D2
P 1625 13425
F 0 "U?" H 1675 13875 50  0001 C CNN
F 1 "SAFETY SWITCH ADJUSTABLE" H 1675 13775 75  0000 C CNB
F 2 "" H 1625 13425 50  0001 C CNN
F 3 "" H 1625 13425 50  0001 C CNN
	1    1625 13425
	-1   0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:Silencer S?
U 1 1 60E6ADA9
P 4375 10275
F 0 "S?" H 3600 10200 50  0001 R CNN
F 1 "SILENCER" H 4650 10075 75  0000 R CNB
F 2 "" H 4375 10275 50  0001 C CNN
F 3 "" H 4375 10275 50  0001 C CNN
	1    4375 10275
	-1   0    0    1   
$EndComp
$Comp
L M19O2_pneumatic:Silencer S?
U 1 1 60E6DC20
P 6350 10275
F 0 "S?" H 7125 10350 50  0001 L CNN
F 1 "SILENCER" H 6075 10475 75  0000 L CNB
F 2 "" H 6350 10275 50  0001 C CNN
F 3 "" H 6350 10275 50  0001 C CNN
	1    6350 10275
	1    0    0    -1  
$EndComp
Wire Bus Line
	2300 14275 4175 14275
Wire Bus Line
	2300 14400 2300 14275
Wire Wire Line
	2300 14400 2300 14275
Wire Wire Line
	2300 14275 4175 14275
Wire Wire Line
	4925 14275 5475 14275
Wire Bus Line
	4925 14275 5475 14275
Wire Wire Line
	8575 14275 8575 13750
Wire Bus Line
	8575 14275 8575 13750
Wire Wire Line
	5475 14275 5475 13425
Wire Wire Line
	5475 13425 2325 13425
Connection ~ 5475 14275
Wire Wire Line
	5475 14275 6025 14275
Wire Bus Line
	5475 14275 6025 14275
Wire Bus Line
	5475 14275 5475 13425
Wire Bus Line
	5475 13425 2325 13425
Wire Bus Line
	5200 9750 5200 10275
Wire Bus Line
	5200 10275 4825 10275
Wire Bus Line
	5500 9750 5500 10275
Wire Bus Line
	5500 10275 5900 10275
Wire Wire Line
	5500 9750 5500 10275
Wire Wire Line
	5500 10275 5900 10275
Wire Wire Line
	5200 9750 5200 10275
Wire Wire Line
	5200 10275 4825 10275
Wire Wire Line
	5200 9050 5200 8800
Wire Wire Line
	5200 8800 3350 8800
Wire Wire Line
	3350 8800 3350 8525
Wire Wire Line
	5500 9050 5500 8800
Wire Wire Line
	5500 8800 8350 8800
Wire Wire Line
	8350 8800 8350 8525
Wire Bus Line
	8350 8525 8350 8800
Wire Bus Line
	8350 8800 5500 8800
Wire Bus Line
	5500 8800 5500 9150
Wire Bus Line
	5200 9150 5200 8800
Wire Bus Line
	5200 8800 3350 8800
Wire Bus Line
	3350 8800 3350 8525
Wire Wire Line
	3350 7275 3350 7100
Wire Wire Line
	3350 7100 3600 7100
Wire Wire Line
	8350 7275 8350 7100
Wire Wire Line
	8350 7100 8100 7100
Wire Wire Line
	7600 7100 5850 7100
Wire Wire Line
	5850 6700 5850 7100
Connection ~ 5850 7100
Wire Wire Line
	5850 7100 4100 7100
Wire Bus Line
	3350 7275 3350 7100
Wire Bus Line
	3350 7100 3600 7100
Wire Bus Line
	4100 7100 5850 7100
Wire Bus Line
	5850 7100 7600 7100
Wire Bus Line
	8100 7100 8350 7100
Wire Bus Line
	8350 7100 8350 7275
Wire Bus Line
	5850 6700 5850 7100
Wire Wire Line
	5850 5575 5850 5700
Wire Bus Line
	5850 5700 5850 5575
Wire Wire Line
	5850 4325 5850 4150
Wire Wire Line
	5850 4150 1925 4150
Wire Wire Line
	1925 4150 1925 2825
Wire Wire Line
	1925 2825 2400 2825
Wire Bus Line
	2400 2825 1925 2825
Wire Bus Line
	1925 2825 1925 4150
Wire Bus Line
	1925 4150 5850 4150
Wire Bus Line
	5850 4150 5850 4325
Wire Wire Line
	3400 2825 4250 2825
Wire Bus Line
	3400 2825 4250 2825
Wire Wire Line
	5000 2825 5325 2825
Wire Bus Line
	5000 2825 5325 2825
Wire Wire Line
	6650 2825 7400 2825
Wire Bus Line
	6650 2825 7400 2825
Wire Bus Line
	7525 3325 8375 3325
Wire Wire Line
	7525 3325 8375 3325
Text GLabel 8375 3325 2    100  Output ~ 20
>90%_O2_OUT
Text GLabel 1850 15850 0    100  Input ~ 20
AMBIENT_AIR
Wire Wire Line
	1850 15850 2300 15850
Wire Wire Line
	2300 15850 2300 15400
Wire Bus Line
	2300 15400 2300 15850
Wire Bus Line
	2300 15850 1850 15850
Wire Wire Line
	1275 14800 1025 14800
Wire Wire Line
	1275 15000 1025 15000
Wire Wire Line
	1625 15275 1625 15325
Wire Wire Line
	1625 15325 1025 15325
Text HLabel 1025 14800 0    50   Input ~ 10
LINE
Text HLabel 1025 15000 0    50   Input ~ 10
NEUT
Text HLabel 1025 15325 0    50   Input ~ 10
EARTH
Wire Wire Line
	6775 14275 8575 14275
Wire Bus Line
	6775 14275 8575 14275
Wire Wire Line
	5350 9750 5350 12025
Wire Bus Line
	5350 9750 5350 12025
Wire Wire Line
	5350 12025 6475 12025
Wire Bus Line
	5350 12025 6475 12025
Wire Wire Line
	7475 12025 8575 12025
Wire Wire Line
	8575 12025 8575 12500
Wire Bus Line
	7475 12025 8575 12025
Wire Bus Line
	8575 12025 8575 12500
Wire Wire Line
	7075 1900 6475 1900
Wire Wire Line
	5675 1900 5325 1900
Wire Wire Line
	5325 1900 5325 2825
Connection ~ 5325 2825
Wire Wire Line
	5325 2825 5650 2825
Wire Bus Line
	5325 2825 5650 2825
Wire Bus Line
	5325 2825 5325 1900
Wire Bus Line
	5325 1900 5675 1900
Wire Bus Line
	6475 1900 7075 1900
Wire Wire Line
	8075 1900 8375 1900
Wire Bus Line
	8075 1900 8375 1900
Text GLabel 8375 1900 2    100  Output ~ 20
BLEED,<0.3LPM
Text Notes 2325 15850 0    75   ~ 0
1800W / 2.5 hp \nZHEJIANG TENGJIGN AIR COMPRESSOR\nModel JB1800\n275 LPM @ 1 bar\n233 LPM @ 4 bar
Text Notes 2300 14200 0    75   ~ 0
2 nos 10mm OD Cu pipe\nEach coil 12 turns, ~~110 mm dia\n1/2" Male coupler for hot end\n3/8" Female coupler for cold end
Text Notes 6800 14225 0    75   ~ 0
Hydint \nModel HPVS Separator\nModel ADB-150 Auto Drain
Text Notes 8875 13825 0    75   ~ 0
Required if Humidity level\nafter previous stage is more\nthan 50% rh\nUse Alumina or Silica Gel\nSize, 75 mm ID x 250 mm tall
Text Notes 775  12975 0    75   ~ 0
Danfoss\nModel KP-35 or equivalent
Text Notes 7475 11975 0    75   ~ 0
Required when testing\nbut not necessary in production
Text Notes 7400 9650 0    75   ~ 0
Use a single 5/3 valve\nor two 3/2 valves\n1/2" or 3/8" ports\n24 VDC coil
Text Notes 4725 8300 0    75   ~ 0
Aluminum extrusion cannisters\n75 mm ID x 500 mm tall\nLiX Zeolite, 0.4 ~~ 0.8 mm bead\n3/8" ports at both ends\nSee Sieve Assembly document for\nconstruction details
Text Notes 8150 6875 0    75   ~ 0
Machined Aluminum or Brass\n(laser cut acrylic works too)\nOD to suit ID of pneumatic pipe\nOrifice bore dia = betn 1.0 to 1.8 mm\nLength, approx 10 mm or more\nAll edges deburred
Text Notes 6150 5275 0    75   ~ 0
Aluminum Extrusion\n75 mm ID x 250 mm tall
Text Notes 2125 3800 0    75   ~ 0
Up to 2 bar range
Text Notes 4275 3800 0    75   ~ 0
Particulate filter
Text Notes 5575 3800 0    75   ~ 0
15 LPM range
Text Notes 7200 3800 0    75   ~ 0
Bubbler bottle
Text Notes 5675 1350 2    75   ~ 0
OOM102 or similar\nElectro-chemical sensor\nconnected to Electronics
Text Notes 6700 1250 0    75   ~ 0
Bleed valve, one way\nset to about less than 0.3 LPM
$EndSCHEMATC