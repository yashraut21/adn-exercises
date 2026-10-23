#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

void setup() {
    Serial.begin(115200);
    WiFi.begin("ADN-IOT", "WBNuyawB2a");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    Serial.println("WiFi connected");
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin("http://192.168.0.51/cm?cmnd=Power%20TOGGLE");

        int status = http.GET();
        Serial.println(status);

        http.end();
    }

    delay(2000);
}