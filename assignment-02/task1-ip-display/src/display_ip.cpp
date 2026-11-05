#include <Arduino.h>
#include <WiFi.h>

static void connectToWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin("ADN-IOT", "WBNuyawB2a");
    Serial.println("Connecting to ADN-IOT...");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("Wi-Fi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void setup() {
    Serial.begin(115200);
    delay(500);
    connectToWiFi();
}

void loop() {
    // Task 1 only needs to report the assigned address after connection.
}
