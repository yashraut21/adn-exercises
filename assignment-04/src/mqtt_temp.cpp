#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <cstring>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

static const char* WIFI_SSID = "ADN-IOT";
static const char* WIFI_PASS = "WBNuyawB2a";

static const char* MQTT_SERVER = "192.168.0.1";
static const uint16_t MQTT_PORT = 1883;
static const char* MQTT_USER = "adn-group43";
static const char* MQTT_TOPIC = "adn/group43/temp";

static const unsigned long PUBLISH_INTERVAL_MS = 5000;
static const unsigned long BLINK_INTERVAL_MS = 250;

WiFiClient espClient;
PubSubClient mqttClient(espClient);
Adafruit_BME280 bme;

unsigned long lastPublishMs = 0;
unsigned long lastBlinkMs = 0;
bool ledState = false;

void updateStatusLed(bool isConnected) {
  if (isConnected) {
    digitalWrite(LED_BUILTIN, HIGH);
    ledState = true;
    return;
  }
  unsigned long now = millis();
  if (now - lastBlinkMs >= BLINK_INTERVAL_MS) {
    lastBlinkMs = now;
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState ? HIGH : LOW);
  }
}

void ensureConnections() {
  while (WiFi.status() != WL_CONNECTED) {
    updateStatusLed(false);
    delay(10);
  }

  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  while (!mqttClient.connected()) {
    updateStatusLed(false);
    if (mqttClient.connect(MQTT_USER, MQTT_USER, nullptr)) {
      break;
    }
    delay(500);
  }

  updateStatusLed(true);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Serial.begin(9600);
  Serial.begin(115200);
  WiFi.setHostname(MQTT_USER);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  if (!bme.begin(0x76)) {
    Serial.println("BME280 not found at 0x76");
  }
}

void loop() {
  ensureConnections();
  mqttClient.loop();

  unsigned long now = millis();
  if (now - lastPublishMs >= PUBLISH_INTERVAL_MS) {
    lastPublishMs = now;

    // float temp = bme.readTemperature();
    // char payload[sizeof(float)];
    // std::memcpy(payload, &temp, sizeof(float));

    // mqttClient.publish(MQTT_TOPIC, reinterpret_cast<const uint8_t*>(payload), sizeof(float), false);

    float temp = bme.readTemperature();
    Serial.print("Temperature: ");
    Serial.print(temp, 2);
    Serial.println(" °C");

    char payload[sizeof(float)];
    std::memcpy(payload, &temp, sizeof(float));

    mqttClient.publish(MQTT_TOPIC, reinterpret_cast<const uint8_t*>(payload), sizeof(float), false);
  }
}