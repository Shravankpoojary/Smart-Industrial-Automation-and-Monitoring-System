# Smart-Industrial-Automation-and-Monitoring-System
Smart industrial automation system using Arduino and ESP8266 with FreeRTOS. It adjusts lighting based on sunlight, user input, and motion detection. Monitors gas leakage and temperature, controls cooling via relay, and publishes sensor data to an MQTT-based IoT dashboard for real-time monitoring.

Tech Stack: NodeMCU ESP8266, MQTTT mosquito broker, Arduino uno -R3, MQ-135 , Arduino IDE, BTA41-600B, PIR sensor,AC Dimmer LED Bulb.

Features:
Gas leak detection & auto-alert 
Temperature-based fan control
Remote appliance control (lights, pump) automatically according to User and Environment Factors
User interface via MQTT IOT Panel App

Overview
A smart lighting system that automatically adjusts the brightness of AC-powered LED lighting using an Arduino Uno. It leverages a potentiometer, LDR (light sensor), PIR (motion sensor), AC dimmer circuit (with BTA41-600B TRIAC and power MOSFETs), and relay module to achieve energy-efficient, user-friendly, and fully automated operation—ideal for industrial environments.


Key Components and Their Roles:
<img width="655" height="572" alt="Screenshot 2025-07-31 100801" src="https://github.com/user-attachments/assets/259f2d59-4c2e-44bc-aab2-5995f85b24d0" />


Main Functional Features:
<img width="928" height="441" alt="Screenshot 2025-07-31 095412" src="https://github.com/user-attachments/assets/8ed31e54-ee5b-486e-8ec5-c805fda3b007" />

<img width="756" height="302" alt="image" src="https://github.com/user-attachments/assets/d679f2f7-61d5-4675-8b1b-244879f833eb" />

The Block Diagram of System :
<img width="856" height="735" alt="image" src="https://github.com/user-attachments/assets/7ada773d-71fe-4393-9080-6fbfbbaf61af" />


How the System Works

Lighting Control (Arduino):
 Manual Mode:

  -Potentiometer values 0–511.
  -User directly controls LED brightness via dimmer.

 Automatic Mode: 
 -Potentiometer values 512–1023.
 -LDR measures sunlight.
 -More sunlight = lower LED brightness.
 -Less sunlight = higher brightness.

Presence Detection:
-PIR sensor detects motion.
-Lights turn ON on motion, turn OFF after no motion for a set timeout (e.g., 30-60 seconds).

Environmental Monitoring & Alerts (ESP8266):
 Temperature Monitoring:
  -DHT11 sensor periodically reads temperature.
  -If temperature exceeds threshold (e.g., 32°C), cooling system turned ON via relay.
  -Cooling turns OFF when temperature normalizes.
  
 Gas Leakage Detection:
  -MQ135 sensor monitors air quality near gas cylinders.
  -If leakage detected (sensor value above threshold), buzzer alerts workers.
  -Multiple buzzers indicate specific leakage location.

IoT Integration:
 -Sensor data (temperature, gas levels) sent via MQTT to a broker.
 -Data displayed on IoT monitoring panel with real-time graphs.
