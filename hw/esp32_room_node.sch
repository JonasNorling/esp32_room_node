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
	2700 3050 2150 3050
Text Label 2800 2900 0    50   ~ 0
LCD_CS
Text Label 2800 3700 0    50   ~ 0
LCD_MOSI
Text Label 2800 2700 0    50   ~ 0
LCD_DC
Text Label 2150 3050 0    50   ~ 0
LCD_BL
Text Label 2800 3600 0    50   ~ 0
LCD_SCK
Wire Wire Line
	6600 2150 7100 2150
Text Label 7100 2150 2    50   ~ 0
SENS_DATA
Wire Wire Line
	2850 3200 3350 3200
Text Label 2850 3200 0    50   ~ 0
SENS_DATA
$EndSCHEMATC
