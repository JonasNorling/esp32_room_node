EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "ESP32 room node"
Date ""
Rev ""
Comp "Jonas Norling"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Module:Adafruit_Feather_HUZZAH32_ESP32 A1
U 1 1 6114F112
P 3850 3500
F 0 "A1" H 3850 2111 50  0000 C CNN
F 1 "Adafruit_Feather_HUZZAH32_ESP32" H 3850 2020 50  0000 C CNN
F 2 "Module:Adafruit_Feather" H 3950 2150 50  0001 L CNN
F 3 "https://cdn-learn.adafruit.com/downloads/pdf/adafruit-huzzah32-esp32-feather.pdf" H 3850 2300 50  0001 C CNN
	1    3850 3500
	1    0    0    -1  
$EndComp
$Comp
L Sensor:DHT11 U1
U 1 1 611508F4
P 6300 2150
F 0 "U1" H 6056 2196 50  0000 R CNN
F 1 "DHT11" H 6056 2105 50  0000 R CNN
F 2 "Sensor:Aosong_DHT11_5.5x12.0_P2.54mm" H 6300 1750 50  0001 C CNN
F 3 "http://akizukidenshi.com/download/ds/aosong/DHT11.pdf" H 6450 2400 50  0001 C CNN
	1    6300 2150
	1    0    0    -1  
$EndComp
$Comp
L Sensor_Humidity:Si7020-A20 U2
U 1 1 61151B33
P 6350 3700
F 0 "U2" H 6794 3746 50  0000 L CNN
F 1 "HTU21" H 6794 3655 50  0000 L CNN
F 2 "Package_DFN_QFN:DFN-6-1EP_3x3mm_P1mm_EP1.5x2.4mm" H 6350 3300 50  0001 C CNN
F 3 "https://www.silabs.com/documents/public/data-sheets/Si7020-A20.pdf" H 6150 4000 50  0001 C CNN
	1    6350 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 611531CC
P 7200 3700
F 0 "C1" H 7315 3746 50  0000 L CNN
F 1 "100n" H 7315 3655 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric_Pad0.74x0.62mm_HandSolder" H 7238 3550 50  0001 C CNN
F 3 "~" H 7200 3700 50  0001 C CNN
	1    7200 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 61153902
P 5700 3350
F 0 "R2" H 5770 3396 50  0000 L CNN
F 1 "R" H 5770 3305 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric_Pad0.72x0.64mm_HandSolder" V 5630 3350 50  0001 C CNN
F 3 "~" H 5700 3350 50  0001 C CNN
	1    5700 3350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 61153DE4
P 5500 3350
F 0 "R1" H 5570 3396 50  0000 L CNN
F 1 "R" H 5570 3305 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric_Pad0.72x0.64mm_HandSolder" V 5430 3350 50  0001 C CNN
F 3 "~" H 5500 3350 50  0001 C CNN
	1    5500 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 4300 2750 4300
Wire Wire Line
	3350 4400 2750 4400
Text Label 2750 4300 0    50   ~ 0
SDA
Text Label 2750 4400 0    50   ~ 0
SCL
Wire Wire Line
	5850 3600 5500 3600
Text Label 5250 3600 0    50   ~ 0
SDA
Text Label 5250 3800 0    50   ~ 0
SCL
Wire Wire Line
	5250 3800 5700 3800
Wire Wire Line
	5500 3500 5500 3600
Connection ~ 5500 3600
Wire Wire Line
	5500 3600 5250 3600
Wire Wire Line
	5700 3500 5700 3800
Connection ~ 5700 3800
Wire Wire Line
	5700 3800 5850 3800
$Comp
L power:+3V3 #PWR0101
U 1 1 6115566C
P 3950 2100
F 0 "#PWR0101" H 3950 1950 50  0001 C CNN
F 1 "+3V3" H 3965 2273 50  0000 C CNN
F 2 "" H 3950 2100 50  0001 C CNN
F 3 "" H 3950 2100 50  0001 C CNN
	1    3950 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 2100 3950 2300
$Comp
L power:+5V #PWR0102
U 1 1 611561B3
P 4050 2200
F 0 "#PWR0102" H 4050 2050 50  0001 C CNN
F 1 "+5V" H 4065 2373 50  0000 C CNN
F 2 "" H 4050 2200 50  0001 C CNN
F 3 "" H 4050 2200 50  0001 C CNN
	1    4050 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 2200 4050 2300
$Comp
L power:+3V3 #PWR0103
U 1 1 611568A4
P 5500 3100
F 0 "#PWR0103" H 5500 2950 50  0001 C CNN
F 1 "+3V3" H 5515 3273 50  0000 C CNN
F 2 "" H 5500 3100 50  0001 C CNN
F 3 "" H 5500 3100 50  0001 C CNN
	1    5500 3100
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR0104
U 1 1 61156EE0
P 5700 3100
F 0 "#PWR0104" H 5700 2950 50  0001 C CNN
F 1 "+3V3" H 5715 3273 50  0000 C CNN
F 2 "" H 5700 3100 50  0001 C CNN
F 3 "" H 5700 3100 50  0001 C CNN
	1    5700 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 3100 5500 3200
Wire Wire Line
	5700 3100 5700 3200
$Comp
L power:+3V3 #PWR0105
U 1 1 61157760
P 6300 1750
F 0 "#PWR0105" H 6300 1600 50  0001 C CNN
F 1 "+3V3" H 6315 1923 50  0000 C CNN
F 2 "" H 6300 1750 50  0001 C CNN
F 3 "" H 6300 1750 50  0001 C CNN
	1    6300 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 1750 6300 1850
$Comp
L power:GND #PWR0106
U 1 1 611580DB
P 6300 2600
F 0 "#PWR0106" H 6300 2350 50  0001 C CNN
F 1 "GND" H 6305 2427 50  0000 C CNN
F 2 "" H 6300 2600 50  0001 C CNN
F 3 "" H 6300 2600 50  0001 C CNN
	1    6300 2600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0107
U 1 1 611584E8
P 6250 4150
F 0 "#PWR0107" H 6250 3900 50  0001 C CNN
F 1 "GND" H 6255 3977 50  0000 C CNN
F 2 "" H 6250 4150 50  0001 C CNN
F 3 "" H 6250 4150 50  0001 C CNN
	1    6250 4150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0108
U 1 1 6115893E
P 3850 5100
F 0 "#PWR0108" H 3850 4850 50  0001 C CNN
F 1 "GND" H 3855 4927 50  0000 C CNN
F 2 "" H 3850 5100 50  0001 C CNN
F 3 "" H 3850 5100 50  0001 C CNN
	1    3850 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 5100 3850 4800
Wire Wire Line
	6250 4000 6250 4050
Wire Wire Line
	6300 2450 6300 2600
$Comp
L power:+3V3 #PWR0109
U 1 1 61159519
P 6350 3200
F 0 "#PWR0109" H 6350 3050 50  0001 C CNN
F 1 "+3V3" H 6365 3373 50  0000 C CNN
F 2 "" H 6350 3200 50  0001 C CNN
F 3 "" H 6350 3200 50  0001 C CNN
	1    6350 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 3200 6350 3350
Wire Wire Line
	6350 3350 7200 3350
Wire Wire Line
	7200 3350 7200 3550
Connection ~ 6350 3350
Wire Wire Line
	6350 3350 6350 3400
Wire Wire Line
	7200 3850 7200 4050
Wire Wire Line
	7200 4050 6250 4050
Connection ~ 6250 4050
Wire Wire Line
	6250 4050 6250 4150
$Comp
L Connector_Generic:Conn_01x07 DISP1
U 1 1 6115ADE6
P 8750 2700
F 0 "DISP1" H 8830 2742 50  0000 L CNN
F 1 "SPI LCD" H 8830 2651 50  0000 L CNN
F 2 "LCD Display:PIM436" H 8750 2700 50  0001 C CNN
F 3 "~" H 8750 2700 50  0001 C CNN
	1    8750 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 2400 8000 2400
Wire Wire Line
	8550 2500 8000 2500
Wire Wire Line
	8550 2600 8000 2600
Wire Wire Line
	8550 2700 8000 2700
Wire Wire Line
	8550 2800 8000 2800
Wire Wire Line
	8550 2900 8000 2900
Wire Wire Line
	8550 3000 8000 3000
Text Label 8000 2400 0    50   ~ 0
LCD_VCC
Text Label 8000 2500 0    50   ~ 0
LCD_CS
Text Label 8000 2700 0    50   ~ 0
LCD_MOSI
Text Label 8000 2800 0    50   ~ 0
LCD_DC
Text Label 8000 2900 0    50   ~ 0
LCD_BL
Text Label 8000 3000 0    50   ~ 0
LCD_GND
Text Label 8000 2600 0    50   ~ 0
LCD_SCK
$Comp
L power:GND #PWR0110
U 1 1 6115F386
P 8000 3100
F 0 "#PWR0110" H 8000 2850 50  0001 C CNN
F 1 "GND" H 8005 2927 50  0000 C CNN
F 2 "" H 8000 3100 50  0001 C CNN
F 3 "" H 8000 3100 50  0001 C CNN
	1    8000 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 3000 8000 3100
$Comp
L power:+5V #PWR0111
U 1 1 6115FB58
P 8000 2250
F 0 "#PWR0111" H 8000 2100 50  0001 C CNN
F 1 "+5V" H 8015 2423 50  0000 C CNN
F 2 "" H 8000 2250 50  0001 C CNN
F 3 "" H 8000 2250 50  0001 C CNN
	1    8000 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 2250 8000 2400
Wire Wire Line
	3350 2900 2800 2900
Wire Wire Line
	3350 3600 2800 3600
Wire Wire Line
	3350 3700 2800 3700
Wire Wire Line
	3350 2700 2800 2700
Wire Wire Line
	3350 2800 2800 2800
Text Label 2800 2900 0    50   ~ 0
LCD_CS
Text Label 2800 3700 0    50   ~ 0
LCD_MOSI
Text Label 2800 2700 0    50   ~ 0
LCD_DC
Text Label 2800 2800 0    50   ~ 0
LCD_BL
Text Label 2800 3600 0    50   ~ 0
LCD_SCK
Wire Wire Line
	6600 2150 7100 2150
Text Label 7100 2150 2    50   ~ 0
SENS_DATA
Wire Wire Line
	2800 3200 3350 3200
Text Label 2800 3200 0    50   ~ 0
SENS_DATA
$Comp
L Device:Rotary_Encoder_Switch SW1
U 1 1 6118E391
P 6300 5250
F 0 "SW1" H 6300 5617 50  0000 C CNN
F 1 "Rotary_Encoder_Switch" H 6300 5526 50  0000 C CNN
F 2 "Rotary_Encoder:RotaryEncoder_Alps_EC11E-Switch_Vertical_H20mm_CircularMountingHoles" H 6150 5410 50  0001 C CNN
F 3 "~" H 6300 5510 50  0001 C CNN
	1    6300 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 5350 6750 5350
Wire Wire Line
	6750 5350 6750 5500
Wire Wire Line
	6000 5250 5950 5250
$Comp
L power:GND #PWR0112
U 1 1 61190D48
P 5950 5450
F 0 "#PWR0112" H 5950 5200 50  0001 C CNN
F 1 "GND" H 5955 5277 50  0000 C CNN
F 2 "" H 5950 5450 50  0001 C CNN
F 3 "" H 5950 5450 50  0001 C CNN
	1    5950 5450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0113
U 1 1 61191003
P 6750 5500
F 0 "#PWR0113" H 6750 5250 50  0001 C CNN
F 1 "GND" H 6755 5327 50  0000 C CNN
F 2 "" H 6750 5500 50  0001 C CNN
F 3 "" H 6750 5500 50  0001 C CNN
	1    6750 5500
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 61191C84
P 5650 4900
F 0 "R5" H 5720 4946 50  0000 L CNN
F 1 "R" H 5720 4855 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric_Pad0.72x0.64mm_HandSolder" V 5580 4900 50  0001 C CNN
F 3 "~" H 5650 4900 50  0001 C CNN
	1    5650 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 5150 5400 5150
Wire Wire Line
	6000 5350 5650 5350
$Comp
L Device:R R4
U 1 1 61193BF5
P 5400 4900
F 0 "R4" H 5470 4946 50  0000 L CNN
F 1 "R" H 5470 4855 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric_Pad0.72x0.64mm_HandSolder" V 5330 4900 50  0001 C CNN
F 3 "~" H 5400 4900 50  0001 C CNN
	1    5400 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 5050 5400 5150
Connection ~ 5400 5150
Wire Wire Line
	5400 5150 5000 5150
Wire Wire Line
	5650 5050 5650 5350
Connection ~ 5650 5350
Wire Wire Line
	5650 5350 5000 5350
Wire Wire Line
	5400 4650 5400 4750
$Comp
L power:+3V3 #PWR0114
U 1 1 611912D9
P 5400 4650
F 0 "#PWR0114" H 5400 4500 50  0001 C CNN
F 1 "+3V3" H 5415 4823 50  0000 C CNN
F 2 "" H 5400 4650 50  0001 C CNN
F 3 "" H 5400 4650 50  0001 C CNN
	1    5400 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 4650 5650 4750
$Comp
L power:+3V3 #PWR0115
U 1 1 61197B24
P 5650 4650
F 0 "#PWR0115" H 5650 4500 50  0001 C CNN
F 1 "+3V3" H 5665 4823 50  0000 C CNN
F 2 "" H 5650 4650 50  0001 C CNN
F 3 "" H 5650 4650 50  0001 C CNN
	1    5650 4650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 61198CAA
P 6900 4900
F 0 "R6" H 6970 4946 50  0000 L CNN
F 1 "R" H 6970 4855 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric_Pad0.72x0.64mm_HandSolder" V 6830 4900 50  0001 C CNN
F 3 "~" H 6900 4900 50  0001 C CNN
	1    6900 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 4650 6900 4750
$Comp
L power:+3V3 #PWR0116
U 1 1 61198ED1
P 6900 4650
F 0 "#PWR0116" H 6900 4500 50  0001 C CNN
F 1 "+3V3" H 6915 4823 50  0000 C CNN
F 2 "" H 6900 4650 50  0001 C CNN
F 3 "" H 6900 4650 50  0001 C CNN
	1    6900 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 5150 6900 5150
Text Label 5000 5150 0    50   ~ 0
ENC_A
Text Label 5000 5350 0    50   ~ 0
ENC_B
Text Label 7400 5150 2    50   ~ 0
ENC_BTN
Wire Wire Line
	6900 5050 6900 5150
Connection ~ 6900 5150
Wire Wire Line
	6900 5150 7400 5150
Text Label 4900 3700 2    50   ~ 0
ENC_B
Text Label 4900 3600 2    50   ~ 0
ENC_A
Text Label 4900 3500 2    50   ~ 0
ENC_BTN
Wire Wire Line
	4350 3700 4900 3700
Wire Wire Line
	4350 3600 4900 3600
Wire Wire Line
	4350 3500 4900 3500
Text Notes 4350 3300 0    50   ~ 0
IO26
Text Notes 4350 3400 0    50   ~ 0
IO25
$Comp
L Connector_Generic:Conn_01x03 J1
U 1 1 611B2274
P 8750 4500
F 0 "J1" H 8830 4542 50  0000 L CNN
F 1 "Conn_01x03" H 8830 4451 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 8750 4500 50  0001 C CNN
F 3 "~" H 8750 4500 50  0001 C CNN
	1    8750 4500
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 611B2858
P 7700 4200
F 0 "R3" H 7770 4246 50  0000 L CNN
F 1 "4k7" H 7770 4155 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric_Pad0.72x0.64mm_HandSolder" V 7630 4200 50  0001 C CNN
F 3 "~" H 7700 4200 50  0001 C CNN
	1    7700 4200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R7
U 1 1 611B2BA7
P 8250 4400
F 0 "R7" H 8320 4446 50  0000 L CNN
F 1 "10" H 8320 4355 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric_Pad0.72x0.64mm_HandSolder" V 8180 4400 50  0001 C CNN
F 3 "~" H 8250 4400 50  0001 C CNN
	1    8250 4400
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0117
U 1 1 611B2D8B
P 8550 4750
F 0 "#PWR0117" H 8550 4500 50  0001 C CNN
F 1 "GND" H 8555 4577 50  0000 C CNN
F 2 "" H 8550 4750 50  0001 C CNN
F 3 "" H 8550 4750 50  0001 C CNN
	1    8550 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 4600 8550 4750
$Comp
L Device:R R8
U 1 1 611B5A78
P 8250 4500
F 0 "R8" H 8320 4546 50  0000 L CNN
F 1 "10" H 8320 4455 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric_Pad0.72x0.64mm_HandSolder" V 8180 4500 50  0001 C CNN
F 3 "~" H 8250 4500 50  0001 C CNN
	1    8250 4500
	0    1    1    0   
$EndComp
Wire Wire Line
	8400 4400 8550 4400
Wire Wire Line
	8550 4500 8400 4500
Wire Wire Line
	8100 4500 7700 4500
Text Label 7700 4500 0    50   ~ 0
ONEWIRE
Wire Wire Line
	7700 4350 7700 4500
$Comp
L power:+3V3 #PWR0118
U 1 1 611BBE87
P 7700 4050
F 0 "#PWR0118" H 7700 3900 50  0001 C CNN
F 1 "+3V3" H 7715 4223 50  0000 C CNN
F 2 "" H 7700 4050 50  0001 C CNN
F 3 "" H 7700 4050 50  0001 C CNN
	1    7700 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 3000 2800 3000
Text Label 2800 3000 0    50   ~ 0
ONEWIRE
Wire Wire Line
	8100 4400 8100 4250
$Comp
L power:+3V3 #PWR0119
U 1 1 611C0FDB
P 8100 4250
F 0 "#PWR0119" H 8100 4100 50  0001 C CNN
F 1 "+3V3" H 8115 4423 50  0000 C CNN
F 2 "" H 8100 4250 50  0001 C CNN
F 3 "" H 8100 4250 50  0001 C CNN
	1    8100 4250
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J2
U 1 1 611C181A
P 8800 5600
F 0 "J2" H 8880 5642 50  0000 L CNN
F 1 "Conn_01x03" H 8880 5551 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 8800 5600 50  0001 C CNN
F 3 "~" H 8800 5600 50  0001 C CNN
	1    8800 5600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R9
U 1 1 611C2222
P 8250 5500
F 0 "R9" H 8320 5546 50  0000 L CNN
F 1 "10" H 8320 5455 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric_Pad0.72x0.64mm_HandSolder" V 8180 5500 50  0001 C CNN
F 3 "~" H 8250 5500 50  0001 C CNN
	1    8250 5500
	0    1    1    0   
$EndComp
Wire Wire Line
	8400 5500 8600 5500
Wire Wire Line
	8100 5500 7500 5500
Text Label 7500 5500 0    50   ~ 0
SERVO
$Comp
L power:+5V #PWR0120
U 1 1 611C6C08
P 8500 5350
F 0 "#PWR0120" H 8500 5200 50  0001 C CNN
F 1 "+5V" H 8515 5523 50  0000 C CNN
F 2 "" H 8500 5350 50  0001 C CNN
F 3 "" H 8500 5350 50  0001 C CNN
	1    8500 5350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0121
U 1 1 611C7053
P 8550 5900
F 0 "#PWR0121" H 8550 5650 50  0001 C CNN
F 1 "GND" H 8555 5727 50  0000 C CNN
F 2 "" H 8550 5900 50  0001 C CNN
F 3 "" H 8550 5900 50  0001 C CNN
	1    8550 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	8600 5700 8550 5700
Wire Wire Line
	8550 5700 8550 5900
Wire Wire Line
	8600 5600 8500 5600
Wire Wire Line
	8500 5600 8500 5350
Wire Wire Line
	3350 3100 2750 3100
Text Label 2750 3100 0    50   ~ 0
SERVO
$Comp
L Device:C C?
U 1 1 611D44A2
P 6900 5450
F 0 "C?" H 7015 5496 50  0000 L CNN
F 1 "DNP" H 7015 5405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric_Pad0.74x0.62mm_HandSolder" H 6938 5300 50  0001 C CNN
F 3 "~" H 6900 5450 50  0001 C CNN
	1    6900 5450
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 611D4DEC
P 5650 5650
F 0 "C?" H 5765 5696 50  0000 L CNN
F 1 "DNP" H 5765 5605 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric_Pad0.74x0.62mm_HandSolder" H 5688 5500 50  0001 C CNN
F 3 "~" H 5650 5650 50  0001 C CNN
	1    5650 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 5450 5950 5250
Wire Wire Line
	5400 5150 5400 5500
Wire Wire Line
	5650 5350 5650 5500
$Comp
L power:GND #PWR?
U 1 1 611D9CA9
P 6900 5700
F 0 "#PWR?" H 6900 5450 50  0001 C CNN
F 1 "GND" H 6905 5527 50  0000 C CNN
F 2 "" H 6900 5700 50  0001 C CNN
F 3 "" H 6900 5700 50  0001 C CNN
	1    6900 5700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 611D9F3D
P 5650 5900
F 0 "#PWR?" H 5650 5650 50  0001 C CNN
F 1 "GND" H 5655 5727 50  0000 C CNN
F 2 "" H 5650 5900 50  0001 C CNN
F 3 "" H 5650 5900 50  0001 C CNN
	1    5650 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 5800 5650 5900
Wire Wire Line
	6900 5600 6900 5700
Wire Wire Line
	6900 5300 6900 5150
$Comp
L Device:C C?
U 1 1 611E1BBF
P 5400 5650
F 0 "C?" H 5515 5696 50  0000 L CNN
F 1 "DNP" H 5515 5605 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric_Pad0.74x0.62mm_HandSolder" H 5438 5500 50  0001 C CNN
F 3 "~" H 5400 5650 50  0001 C CNN
	1    5400 5650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 611E1F01
P 5400 5900
F 0 "#PWR?" H 5400 5650 50  0001 C CNN
F 1 "GND" H 5405 5727 50  0000 C CNN
F 2 "" H 5400 5900 50  0001 C CNN
F 3 "" H 5400 5900 50  0001 C CNN
	1    5400 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 5800 5400 5900
$EndSCHEMATC
