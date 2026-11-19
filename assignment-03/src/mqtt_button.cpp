#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <AsyncMqttClient.h>
#include <avdweb_Switch.h>
#include <chrono>

using namespace std::chrono;

#define PUSHBUTTON 17
#define MQTT_HOST IPAddress(192, 168, 0, 1) // Verify 192.168.0.1 (ass03) vs 192.168.0.254 (ex03)
#define MQTT_PORT 1883

const char* WIFI_SSID = "ADN-IOT";
const char* WIFI_PASSWORD = "WBNuyawB2a";
const char* LIGHT_TOPIC = "cmnd/sonoff_06/POWER";

AsyncMqttClient mqttClient;
Switch button(PUSHBUTTON);
auto connectStartTime = steady_clock::now();

void connectMqttIfNeeded() {
    static auto lastMqttAttempt = steady_clock::now() - seconds(5);
    if (WiFi.status() == WL_CONNECTED && !mqttClient.connected()) {
        auto now = steady_clock::now();
        if (now - lastMqttAttempt >= seconds(2)) {
            lastMqttAttempt = now;
            mqttClient.connect();
        }
    }
}

void updateStatusLed() {
    static auto lastToggle = steady_clock::now();
    bool linkAndMqttReady = (WiFi.status() == WL_CONNECTED) && mqttClient.connected();

    if (linkAndMqttReady) {
        digitalWrite(LED_BUILTIN, HIGH);
        return;
    }

    auto elapsed = steady_clock::now() - connectStartTime;
    auto blinkInterval = (elapsed < seconds(10)) ? milliseconds(500) : milliseconds(100);

    auto now = steady_clock::now();
    if (now - lastToggle >= blinkInterval) {
        lastToggle = now;
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
}

void onMqttConnect(bool sessionPresent) {
    Serial.println("MQTT connected");
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    connectStartTime = steady_clock::now();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        updateStatusLed();
        delay(20);
    }
    Serial.println("\nWiFi connected");

    mqttClient.setClientId("ADNGROUP");
    mqttClient.setServer(MQTT_HOST, MQTT_PORT);
    mqttClient.onConnect(onMqttConnect);
    mqttClient.connect();
}

void loop() {
    button.poll();
    connectMqttIfNeeded();
    updateStatusLed();

    if (button.pushed()) {
        Serial.println("Button pressed");
        if (mqttClient.connected()) {
            mqttClient.publish(LIGHT_TOPIC, 0, false, "TOGGLE", 6);
            Serial.println("TOGGLE sent");
        } else {
            Serial.println("MQTT not connected; TOGGLE skipped");
        }
    }
}