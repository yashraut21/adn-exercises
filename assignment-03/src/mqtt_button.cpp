#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <AsyncMqttClient.h>
#include <avdweb_Switch.h>

#define PUSHBUTTON 17
#define MQTT_HOST IPAddress(192,168,0,254)
#define MQTT_PORT 1883

const char* WIFI_SSID = "ADN-IOT";
const char* WIFI_PASSWORD = "WBNuyawB2a";
const char* LIGHT_TOPIC = "cmnd/REPLACE_WITH_SWITCH_ID/POWER";

AsyncMqttClient mqttClient;
Switch button(PUSHBUTTON);

void onMqttConnect(bool sessionPresent) {
    Serial.println("MQTT connected");
}

void setup() {
    Serial.begin(115200);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");

    mqttClient.setClientId("ADNGROUP");
    mqttClient.setServer(MQTT_HOST, MQTT_PORT);
    mqttClient.onConnect(onMqttConnect);
    mqttClient.connect();

    while (!mqttClient.connected()) {
        delay(100);
    }
}

void loop() {
    button.poll();

    if (button.pushed()) {
        Serial.println("Button pressed");
        if (mqttClient.connected()) {
            mqttClient.publish(LIGHT_TOPIC, 0, false, "TOGGLE", 6);
            Serial.println("TOGGLE sent");
        }
    }
}
