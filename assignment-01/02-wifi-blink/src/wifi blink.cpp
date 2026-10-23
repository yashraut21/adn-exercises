#include <Arduino.h>
#include <WiFi.h>
#include <chrono>

using namespace std::chrono;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    WiFi.begin("ADN-IOT", "WBNuyawB2a");
    // WiFi.begin("ADN-IOT", "wrongpass"); // uncomment and upload with wrong credentials
}

void loop() {
    static auto startTime = steady_clock::now();

    if (WiFi.status() == WL_CONNECTED) {
        digitalWrite(LED_BUILTIN, HIGH);
    } else if (steady_clock::now() - startTime < seconds(10)) {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        delay(500); // long blink
    } else {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        delay(100); // short blink
    }
}