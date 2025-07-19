#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// WiFi credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// MQTT Broker
const char* mqtt_server = "YOUR_MQTT_BROKER_IP"; // e.g. 192.168.1.100
const char* tempTopic = "industry/temperature";
const char* gasTopic = "industry/gas";

// Pins
#define DHTPIN D4
#define DHTTYPE DHT11
#define MQ135_PIN A0
#define COOLING_RELAY D1
#define BUZZER_ZONE1 D2
#define BUZZER_ZONE2 D3

// Thresholds
const float TEMP_HIGH = 30.0;   // Cooling turns ON
const float TEMP_NORMAL = 28.0; // Cooling turns OFF
const int GAS_THRESHOLD = 400;  // Arbitrary analog value – tune as needed

// Objects
WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);

// Variables
bool coolingOn = false;

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP8266Client")) {
      // No subscription needed here; only publishing
    } else {
      delay(1000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(COOLING_RELAY, OUTPUT);
  pinMode(BUZZER_ZONE1, OUTPUT);
  pinMode(BUZZER_ZONE2, OUTPUT);

  digitalWrite(COOLING_RELAY, LOW);
  digitalWrite(BUZZER_ZONE1, LOW);
  digitalWrite(BUZZER_ZONE2, LOW);

  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float temp = dht.readTemperature();
  int gasValue = analogRead(MQ135_PIN);

  // --- MQTT Publish ---
  char tempStr[8];
  dtostrf(temp, 6, 2, tempStr);
  client.publish(tempTopic, tempStr);

  char gasStr[8];
  itoa(gasValue, gasStr, 10);
  client.publish(gasTopic, gasStr);

  // --- Cooling Control ---
  if (temp >= TEMP_HIGH && !coolingOn) {
    digitalWrite(COOLING_RELAY, HIGH);
    coolingOn = true;
  } else if (temp <= TEMP_NORMAL && coolingOn) {
    digitalWrite(COOLING_RELAY, LOW);
    coolingOn = false;
  }

  // --- Gas Leakage Detection ---
  if (gasValue > GAS_THRESHOLD) {
    // Trigger buzzers based on zones (you can customize this logic)
    digitalWrite(BUZZER_ZONE1, HIGH);
    digitalWrite(BUZZER_ZONE2, LOW); // Assume zone1 is leaking here
  } else {
    digitalWrite(BUZZER_ZONE1, LOW);
    digitalWrite(BUZZER_ZONE2, LOW);
  }

  delay(2000); // Avoid flooding MQTT broker
}
