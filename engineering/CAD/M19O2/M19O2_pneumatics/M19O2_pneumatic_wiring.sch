EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A3 11693 16535 portrait
encoding utf-8
Sheet 1 1
Title "M19O2 Pneumatic Circuit"
Date "2021-07-16"
Rev "v2"
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
P 6975 12275
F 0 "PR?" H 7175 13400 50  0001 C CNN
F 1 "HIGH PRESSURE REGULATOR WITH GAUGE (OPTION)" H 5025 12675 75  0000 C CNB
F 2 "" H 6975 12275 50  0001 C CNN
F 3 "" H 6975 12275 50  0001 C CNN
	1    6975 12275
	-1   0    0    -1  
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
P 3350 8575
F 0 "T?" H 3650 8700 50  0001 L CNN
F 1 "LiX MOL SIEVE A" H 3650 8575 75  0000 L CNB
F 2 "" H 3350 8575 50  0001 C CNN
F 3 "" H 3350 8575 50  0001 C CNN
	1    3350 8575
	1    0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:Reservoir T?
U 1 1 60E4D726
P 8350 8575
F 0 "T?" H 8650 8700 50  0001 L CNN
F 1 "LiX MOL SIEVE B" H 7100 8575 75  0000 L CNB
F 2 "" H 8350 8575 50  0001 C CNN
F 3 "" H 8350 8575 50  0001 C CNN
	1    8350 8575
	1    0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:Orifice O?
U 1 1 60E4F664
P 3850 7775
F 0 "O?" H 3850 8150 50  0001 C CNN
F 1 "ORIFICE" H 3850 8050 75  0000 C CNB
F 2 "" H 3850 7775 50  0001 C CNN
F 3 "" H 3850 7775 50  0001 C CNN
	1    3850 7775
	1    0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:Orifice O?
U 1 1 60E5046F
P 7850 7775
F 0 "O?" H 7850 8150 50  0001 C CNN
F 1 "ORIFICE" H 7850 8050 75  0000 C CNB
F 2 "" H 7850 7775 50  0001 C CNN
F 3 "" H 7850 7775 50  0001 C CNN
	1    7850 7775
	1    0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:Valve_non_return U?
U 1 1 60E547FE
P 5850 6925
F 0 "U?" V 5825 6600 50  0001 R CNN
F 1 "VALVE, NON-RETURN" V 5850 6600 75  0000 R CNB
F 2 "" H 5850 6925 50  0001 C CNN
F 3 "" H 5850 6925 50  0001 C CNN
	1    5850 6925
	0    -1   -1   0   
$EndComp
$Comp
L M19O2_pneumatic:Reservoir T?
U 1 1 60E55320
P 5850 5650
F 0 "T?" H 6150 5775 50  0001 L CNN
F 1 "SURGE TANK" H 6150 5625 75  0000 L CNB
F 2 "" H 5850 5650 50  0001 C CNN
F 3 "" H 5850 5650 50  0001 C CNN
	1    5850 5650
	1    0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:PrRegGauge PR?
U 1 1 60E568C7
P 2900 3575
F 0 "PR?" H 3100 4700 50  0001 C CNN
F 1 "LOW PRESSURE REGULATOR" H 2900 2800 75  0000 C CNB
F 2 "" H 2900 3575 50  0001 C CNN
F 3 "" H 2900 3575 50  0001 C CNN
	1    2900 3575
	1    0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:Filter F?
U 1 1 60E5B5E7
P 4625 3575
F 0 "F?" H 4650 4150 50  0001 C CNN
F 1 "FILTER HEPA" H 4625 2800 75  0000 C CNB
F 2 "" H 4625 3575 50  0001 C CNN
F 3 "" H 4625 3575 50  0001 C CNN
	1    4625 3575
	1    0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:Flow_Reg_uni_dir O?
U 1 1 60E5C14C
P 7525 1650
F 0 "O?" H 7575 2350 50  0001 C CNN
F 1 "BLEED VALVE, UNI-DIRECTION" H 7525 2600 75  0000 C CNB
F 2 "" H 7525 1650 50  0001 C CNN
F 3 "" H 7525 1650 50  0001 C CNN
	1    7525 1650
	1    0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:OOM102_flow U?
U 1 1 60E60645
P 6075 1050
F 0 "U?" V 7100 1075 50  0001 C CNN
F 1 "O2 SENSOR" V 5725 325 75  0000 C CNB
F 2 "" H 6075 1050 50  0001 C CNN
F 3 "" H 6075 1050 50  0001 C CNN
	1    6075 1050
	0    1    1    0   
$EndComp
$Comp
L M19O2_pneumatic:flow_meter U?
U 1 1 60E63014
P 6150 3575
F 0 "U?" H 6175 4025 50  0001 C CNN
F 1 "ROTAMETER, 15 LPM" H 6150 2800 75  0000 C CNB
F 2 "" H 6300 3575 50  0001 C CNN
F 3 "" H 6300 3575 50  0001 C CNN
	1    6150 3575
	1    0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:Humidifier H?
U 1 1 60E64521
P 7525 3575
F 0 "H?" H 7500 3875 50  0001 C CNN
F 1 "HUMIDIFIER" H 7525 2800 75  0000 C CNB
F 2 "" H 7525 3575 50  0001 C CNN
F 3 "" H 7525 3575 50  0001 C CNN
	1    7525 3575
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
Wire Wire Line
	3350 7950 3350 7775
Wire Wire Line
	3350 7775 3600 7775
Wire Wire Line
	8350 7950 8350 7775
Wire Wire Line
	8350 7775 8100 7775
Wire Wire Line
	7600 7775 5850 7775
Wire Wire Line
	5850 7425 5850 7775
Connection ~ 5850 7775
Wire Wire Line
	5850 7775 4100 7775
Wire Bus Line
	3350 7950 3350 7775
Wire Bus Line
	3350 7775 3600 7775
Wire Bus Line
	4100 7775 5850 7775
Wire Bus Line
	5850 7775 7600 7775
Wire Bus Line
	8100 7775 8350 7775
Wire Bus Line
	8350 7775 8350 7950
Wire Bus Line
	5850 7425 5850 7775
Wire Wire Line
	5850 6275 5850 6425
Wire Bus Line
	5850 6425 5850 6275
Wire Wire Line
	5850 5025 5850 4850
Wire Wire Line
	5850 4850 1925 4850
Wire Wire Line
	1925 4850 1925 3575
Wire Wire Line
	1925 3575 2400 3575
Wire Bus Line
	2400 3575 1925 3575
Wire Bus Line
	1925 3575 1925 4850
Wire Bus Line
	1925 4850 5850 4850
Wire Bus Line
	5850 4850 5850 5025
Wire Wire Line
	3400 3575 4250 3575
Wire Bus Line
	3400 3575 4250 3575
Wire Wire Line
	6650 3575 7400 3575
Wire Bus Line
	6650 3575 7400 3575
Wire Bus Line
	7525 4075 8375 4075
Wire Wire Line
	7525 4075 8375 4075
Text GLabel 8375 4075 2    100  Output ~ 20
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
	5350 12275 6475 12275
Wire Bus Line
	5350 12275 6475 12275
Wire Wire Line
	7475 12275 8575 12275
Wire Wire Line
	8575 12275 8575 12500
Wire Bus Line
	7475 12275 8575 12275
Wire Bus Line
	8575 12275 8575 12500
Wire Wire Line
	7075 1650 6475 1650
Wire Wire Line
	5675 1650 5325 1650
Wire Bus Line
	5325 3575 5650 3575
Wire Bus Line
	5325 1650 5675 1650
Wire Bus Line
	6475 1650 7075 1650
Wire Wire Line
	8075 1650 8375 1650
Wire Bus Line
	8075 1650 8375 1650
Text GLabel 8375 1650 2    100  Output ~ 20
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
Text Notes 7475 12225 0    75   ~ 0
Required when testing\nbut not necessary in production
Text Notes 4725 8975 0    75   ~ 0
Aluminum extrusion cannisters\n75 mm ID x 500 mm tall\nLiX Zeolite, 0.4 ~~ 0.8 mm bead\n3/8" ports at both ends\nSee Sieve Assembly document for\nconstruction details
Text Notes 8125 7700 0    75   ~ 0
Machined Aluminum or Brass\n(laser cut acrylic works too)\nOD to suit ID of pneumatic pipe\nOrifice bore dia = betn 1.0 to 1.8 mm\nLength, approx 10 mm or more\nAll edges deburred
Text Notes 6175 5975 0    75   ~ 0
Aluminum Extrusion\n75 mm ID x 250 mm tall
Text Notes 2125 4550 0    75   ~ 0
Up to 2 bar range
Text Notes 4275 4550 0    75   ~ 0
Particulate filter
Text Notes 5575 4550 0    75   ~ 0
15 LPM range
Text Notes 7200 4550 0    75   ~ 0
Bubbler bottle
Text Notes 5675 1100 2    75   ~ 0
OOM102 or similar\nElectro-chemical sensor\nconnected to Electronics
Text Notes 6700 1000 0    75   ~ 0
Bleed valve, one way\nset to about less than 0.3 LPM
$Comp
L M19O2_pneumatic:Orifice O?
U 1 1 60F1C1D2
P 5325 2525
F 0 "O?" H 5325 2900 50  0001 C CNN
F 1 "ORIFICE" V 5225 2950 75  0000 C CNB
F 2 "" H 5325 2525 50  0001 C CNN
F 3 "" H 5325 2525 50  0001 C CNN
	1    5325 2525
	0    1    1    0   
$EndComp
Text Notes 7400 10250 0    75   ~ 0
Use a single 5/3 valve\nor two 3/2 valves\n1/2" or 3/8" ports\n24 VDC coil
Wire Bus Line
	5350 10350 5350 12275
Wire Wire Line
	5350 10350 5350 12275
Wire Bus Line
	3350 9400 3350 9200
Wire Bus Line
	5200 9400 3350 9400
Wire Bus Line
	5200 9750 5200 9400
Wire Bus Line
	5500 9400 5500 9750
Wire Bus Line
	8350 9400 5500 9400
Wire Bus Line
	8350 9200 8350 9400
Wire Wire Line
	8350 9400 8350 9200
Wire Wire Line
	5500 9400 8350 9400
Wire Wire Line
	5500 9650 5500 9400
Wire Wire Line
	3350 9400 3350 9200
Wire Wire Line
	5200 9400 3350 9400
Wire Wire Line
	5200 9650 5200 9400
Wire Wire Line
	5200 10875 4825 10875
Wire Wire Line
	5200 10350 5200 10875
Wire Wire Line
	5500 10875 5900 10875
Wire Wire Line
	5500 10350 5500 10875
Wire Bus Line
	5500 10875 5900 10875
Wire Bus Line
	5500 10350 5500 10875
Wire Bus Line
	5200 10875 4825 10875
Wire Bus Line
	5200 10350 5200 10875
$Comp
L M19O2_pneumatic:Silencer S?
U 1 1 60E6DC20
P 6350 10875
F 0 "S?" H 7125 10950 50  0001 L CNN
F 1 "SILENCER" H 6075 11075 75  0000 L CNB
F 2 "" H 6350 10875 50  0001 C CNN
F 3 "" H 6350 10875 50  0001 C CNN
	1    6350 10875
	1    0    0    -1  
$EndComp
$Comp
L M19O2_pneumatic:Silencer S?
U 1 1 60E6ADA9
P 4375 10875
F 0 "S?" H 3600 10800 50  0001 R CNN
F 1 "SILENCER" H 4650 10675 75  0000 R CNB
F 2 "" H 4375 10875 50  0001 C CNN
F 3 "" H 4375 10875 50  0001 C CNN
	1    4375 10875
	-1   0    0    1   
$EndComp
$Comp
L M19O2_pneumatic:Valve_5_3_Solenoid V?
U 1 1 60E2A646
P 5850 10000
F 0 "V?" H 4300 10050 50  0001 R CNN
F 1 "VALVE 5/3 24V SOLENOID" H 8875 10300 75  0000 R CNB
F 2 "" H 5850 10000 50  0001 C CNN
F 3 "" H 5850 10000 50  0001 C CNN
	1    5850 10000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 3575 5325 3575
Wire Wire Line
	5325 2275 5325 1650
Wire Wire Line
	5325 2775 5325 3575
Connection ~ 5325 3575
Wire Wire Line
	5325 3575 5650 3575
Wire Bus Line
	5000 3575 5325 3575
Wire Bus Line
	5325 3575 5325 2750
Wire Bus Line
	5325 2275 5325 1650
Text Notes 5525 3225 0    75   ~ 0
Machined Aluminum or Brass\n(laser cut acrylic works too)\nOD to suit ID of pneumatic pipe\nOrifice bore dia = betn 1.0 to 1.2 mm\nLength, approx 10 mm or more\nAll edges deburred
$EndSCHEMATC
