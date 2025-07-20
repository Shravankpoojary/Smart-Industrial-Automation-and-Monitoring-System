#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// === Pin Definitions ===
#define PIR_PIN     2        // Digital input
#define DIMMER_PWM  5        // OC0B (PWM via Timer0)
#define POT_CHANNEL 0        // ADC0
#define LDR_CHANNEL 1        // ADC1

// === Shared Variables ===
volatile uint16_t potValue = 0;
volatile uint16_t ldrValue = 0;
volatile bool pirDetected = false;
unsigned long lastMotionTime = 0;
const unsigned long motionTimeout = 30000;

SemaphoreHandle_t xSensorSemaphore;

// === Register-Level ADC Initialization ===
void initADC() {
  ADMUX = (1 << REFS0);                     // AVcc as reference
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);  // Enable ADC, prescaler = 64
}

// === Read ADC Channel ===
uint16_t readADC(uint8_t channel) {
  ADMUX = (ADMUX & 0xF0) | (channel & 0x0F); // Select channel
  ADCSRA |= (1 << ADSC);                    // Start conversion
  while (ADCSRA & (1 << ADSC));             // Wait for complete
  return ADC;
}

// === PWM Initialization (Timer0) ===
void initPWM() {
  pinMode(DIMMER_PWM, OUTPUT);
  TCCR0A = (1 << COM0B1) | (1 << WGM01) | (1 << WGM00); // Fast PWM, OC0B
  TCCR0B = (1 << CS01);  // Prescaler = 8
}

// === Set PWM Duty ===
void setPWMDuty(uint8_t duty) {
  OCR0B = duty;
}

// === PIR Task ===
void readPIRTask(void *pvParameters) {
  pinMode(PIR_PIN, INPUT);
  while (1) {
    if (digitalRead(PIR_PIN)) {
      pirDetected = true;
      lastMotionTime = millis();
    } else if (millis() - lastMotionTime > motionTimeout) {
      pirDetected = false;
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// === Potentiometer ADC Task ===
void readPotTask(void *pvParameters) {
  while (1) {
    potValue = readADC(POT_CHANNEL);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// === LDR ADC Task ===
void readLDRTask(void *pvParameters) {
  while (1) {
    ldrValue = readADC(LDR_CHANNEL);
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

// === Brightness Control Task ===
void brightnessTask(void *pvParameters) {
  while (1) {
    uint8_t brightness = 0;

    if (pirDetected) {
      if (potValue < 512) {
        brightness = potValue / 2; // Manual mode
      } else {
        brightness = map(ldrValue, 0, 1023, 255, 0); // Auto mode
      }
    } else {
      brightness = 0;
    }

    setPWMDuty(brightness);
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void setup() {
  initADC();
  initPWM();
  xSensorSemaphore = xSemaphoreCreateMutex();

  xTaskCreate(readPIRTask, "PIR", 128, NULL, 1, NULL);
  xTaskCreate(readPotTask, "Pot", 128, NULL, 1, NULL);
  xTaskCreate(readLDRTask, "LDR", 128, NULL, 1, NULL);
  xTaskCreate(brightnessTask, "PWM", 128, NULL, 2, NULL);
}

void loop() {
  // RTOS manages everything
}
