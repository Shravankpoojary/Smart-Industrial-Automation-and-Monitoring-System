# Smart-Industrial-Automation-and-Monitoring-System
Smart industrial automation system using Arduino and ESP8266 with FreeRTOS. It adjusts lighting based on sunlight, user input, and motion detection. Monitors gas leakage and temperature, controls cooling via relay, and publishes sensor data to an MQTT-based IoT dashboard for real-time monitoring.

Tech Stack: NodeMCU ESP8266, MQTTT mosquito broker, Arduino uno -R3, MQ-135 , Arduino IDE, BTA41-600B, PIR sensor.

Features:
Gas leak detection & auto-alert 
Temperature-based fan control
Remote appliance control (lights, pump) automatically according to User and Environment Factors
User interface via MQTT IOT Panel App

Overview
A smart lighting system that automatically adjusts the brightness of AC-powered LED lighting using an Arduino Uno. It leverages a potentiometer, LDR (light sensor), PIR (motion sensor), AC dimmer circuit (with BTA41-600B TRIAC and power MOSFETs), and relay module to achieve energy-efficient, user-friendly, and fully automated operation—ideal for industrial environments.

Feature	What It Does
Potentiometer Control	Acts as a mode selector: manual (left range) or automatic (right range).
Manual Brightness Mode	Left half of potentiometer (0â€“511): user directly sets and controls LED brightness.
Automatic Brightness Mode	Right half (512â€“1023): LDR sensor automatically adjusts LED brightnessâ€”brighter ambient light = dimmer LED.
PIR Motion Detection	Detects presence/motion; if no motion for 30 seconds, LED turns off, else remains on.
FreeRTOS Multitasking	Enables independent and timely tasks for potentiometer, LDR, PIR, and brightness control for smooth operation.
![Uploading image.png…]()

Features:
Dual-Mode Operation:
Manual Mode: Potentiometer directly controls AC LED brightness.

Automatic (Daylight Saving) Mode: LDR sensor adjusts brightness based on ambient sunlight to maximize energy savings during the day.

Presence Detection:
PIR sensor activates lighting when motion is detected (configurable timeout, e.g., 1 min).
Lights turn off automatically after no motion for the preset period.

Robust AC Control:
AC dimmer circuit with BTA41-600B TRIAC and Power MOSFETs ensures silent, smooth, and efficient dimming.

Hardware Requiremnts:
