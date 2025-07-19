#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// Pin configuration
const int potPin = A0;
const int ldrPin = A1;
const int pirPin = 2;
const int dimmerPin = 5;  // PWM pin connected to MOSFET/AC dimmer

// Variables
volatile int potValue = 0;
volatile int ldrValue = 0;
volatile bool pirDetected = false;
unsigned long lastMotionTime = 0;
const unsigned long motionTimeout = 30000; // 30 sec

SemaphoreHandle_t xSensorSemaphore;

// === TASK: Read Potentiometer ===
void readPotTask(void *pvParameters) {
  while (1) {
    potValue = analogRead(potPin);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// === TASK: Read LDR Sensor ===
void readLDRTask(void *pvParameters) {
  while (1) {
    ldrValue = analogRead(ldrPin);
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

// === TASK: PIR Sensor Logic ===
void readPIRTask(void *pvParameters) {
  pinMode(pirPin, INPUT);
  while (1) {
    if (digitalRead(pirPin) == HIGH) {
      pirDetected = true;
      lastMotionTime = millis();
    }
    else if (millis() - lastMotionTime > motionTimeout) {
      pirDetected = false;
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// === TASK: Brightness Controller ===
void brightnessTask(void *pvParameters) {
  pinMode(dimmerPin, OUTPUT);
  while (1) {
    int brightness = 0;

    if (pirDetected) {
      if (potValue < 512) {
        // Manual control
        brightness = map(potValue, 0, 511, 0, 255);
      } else {
        // Auto mode using LDR
        brightness = map(ldrValue, 0, 1023, 255, 0); // More sunlight = less brightness
      }
    } else {
      brightness = 0; // No motion detected
    }

    analogWrite(dimmerPin, brightness);
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void setup() {
  xSensorSemaphore = xSemaphoreCreateMutex();

  // Create tasks
  xTaskCreate(readPotTask, "PotTask", 128, NULL, 1, NULL);
  xTaskCreate(readLDRTask, "LDRTask", 128, NULL, 1, NULL);
  xTaskCreate(readPIRTask, "PIRTask", 128, NULL, 1, NULL);
  xTaskCreate(brightnessTask, "ControlTask", 128, NULL, 2, NULL);
}

void loop() {
  // Not used—FreeRTOS handles tasks
}
