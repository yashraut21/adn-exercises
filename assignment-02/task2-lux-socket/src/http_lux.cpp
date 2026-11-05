#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

// Course Wi-Fi credentials.
static const char* WIFI_SSID = "ADN-IOT";
static const char* WIFI_PASSWORD = "WBNuyawB2a";

// Replace these placeholders with the actual smart-socket API endpoints.
static const char* SOCKET_ON_URL = "http://<socket-ip-or-host>/<on-endpoint>";
static const char* SOCKET_OFF_URL = "http://<socket-ip-or-host>/<off-endpoint>";

// Example thresholds; calibrate them using measured illuminance.
static constexpr float LUX_LOW_THRESHOLD = 100.0f;
static constexpr float LUX_HIGH_THRESHOLD = 300.0f;

Adafruit_TSL2561_Unified tsl(TSL2561_ADDR_FLOAT, 12345);

enum class SocketState { OFF, ON };
SocketState socketState = SocketState::OFF; // Assignment says assume OFF at startup.

static void connectToWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.printf("Connecting to %s", WIFI_SSID);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("Wi-Fi connected. IP: ");
    Serial.println(WiFi.localIP());
}

static bool sendSocketCommand(SocketState target) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Wi-Fi disconnected; socket command not sent.");
        return false;
    }

    const char* url = (target == SocketState::ON) ? SOCKET_ON_URL : SOCKET_OFF_URL;
    WiFiClient client;
    HTTPClient http;

    if (!http.begin(client, url)) {
        Serial.println("Could not initialise HTTP connection.");
        return false;
    }

    const int statusCode = http.GET(); // Explicit ON/OFF URL; never TOGGLE.
    Serial.printf("HTTP response: %d\n", statusCode);
    http.end();
    return statusCode >= 200 && statusCode < 300;
}

static void updateSocket(float lux) {
    SocketState requestedState = socketState;

    if (lux < LUX_LOW_THRESHOLD) requestedState = SocketState::ON;
    else if (lux > LUX_HIGH_THRESHOLD) requestedState = SocketState::OFF;

    // Hysteresis band: retain current state and send no request.
    if (requestedState == socketState) return;

    Serial.printf("Lux threshold crossed. Requesting socket %s.\n",
                  requestedState == SocketState::ON ? "ON" : "OFF");

    // Update the state only after a successful HTTP response.
    if (sendSocketCommand(requestedState)) socketState = requestedState;
    else Serial.println("Socket state unchanged; will retry on a later sensor cycle.");
}

void setup() {
    Serial.begin(115200);
    delay(500);
    connectToWiFi();

    if (!tsl.begin()) {
        Serial.println("TSL2561 not detected. Check wiring and I2C connection.");
        while (true) delay(1000);
    }

    tsl.enableAutoRange(true);
    tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);
    Serial.println("TSL2561 ready. Monitoring illuminance...");
    Serial.println("Initial assumed socket state: OFF");
}

void loop() {
    sensors_event_t event;
    tsl.getEvent(&event);

    if (event.light == 0) {
        Serial.println("Lux reading unavailable or sensor saturated.");
    } else {
        Serial.printf("Illuminance: %.2f lux | Assumed socket: %s\n",
                      event.light,
                      socketState == SocketState::ON ? "ON" : "OFF");
        updateSocket(event.light);
    }

    delay(1000);
}
