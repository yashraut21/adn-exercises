
#include <Arduino.h>
#include <WiFi.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 0
#define LED_PIN LED_BUILTIN
#define LED_ON HIGH
#define LED_OFF LOW

const char* WIFI_SSID = "ADN-IOT";
const char* WIFI_PASSWORD = "WBNuyawB2a";

const unsigned long BLINK_INTERVAL = 500;
const unsigned long RECONNECT_INTERVAL = 10000;

Adafruit_SSD1306 display(OLED_RESET);

bool oledReady = false;
bool wasConnected = false;
bool ledState = false;

unsigned long lastBlink = 0;
unsigned long lastReconnectAttempt = 0;

String displayedIP = "";

void showMessage(const char* line1,const char* line2 = "",const char* line3 = "") {
    if (!oledReady) return;

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println(line1);

    if (line2[0] != '\0') display.println(line2);
    if (line3[0] != '\0') display.println(line3);

    display.display();
}

void showConnected() {
    if (!oledReady) return;

    String ip = WiFi.localIP().toString();

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("WiFi Connected");
    display.print("IP:");
    display.println(ip);
    display.display();

    displayedIP = ip;
}

void setup() {
    pinMode(LED_PIN,OUTPUT);
    digitalWrite(LED_PIN,LED_OFF);

    Serial.begin(9600);
    delay(200);

    // Initialize OLED using course-specified API
    display.begin(SSD1306_SWITCHCAPVCC,0x3C);
    oledReady = true;

    display.display();
    delay(1000);
    showMessage("Starting WiFi...");

    // Initialize WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID,WIFI_PASSWORD);

    Serial.println("Connecting to WiFi...");
}

void loop() {
    bool connected = WiFi.status() == WL_CONNECTED;

    if (connected) {
        digitalWrite(LED_PIN,LED_ON);

        String currentIP = WiFi.localIP().toString();

        if (!wasConnected || currentIP != displayedIP) {
            Serial.println("WiFi connected!");
            Serial.print("IP Address: ");
            Serial.println(currentIP);
            showConnected();
        }

        wasConnected = true;

    } else {
        wasConnected = false;

        // Blink LED while disconnected
        if (millis() - lastBlink >= BLINK_INTERVAL) {
            lastBlink = millis();
            ledState = !ledState;
            digitalWrite(LED_PIN,ledState ? LED_ON : LED_OFF);
        }

        // Update OLED status
        if (displayedIP.length() > 0 || millis() < 2000) {
            displayedIP = "";
            showMessage("WiFi","Disconnected","Reconnecting...");
        }

        // Retry connection periodically
        if (millis() - lastReconnectAttempt >= RECONNECT_INTERVAL) {
            lastReconnectAttempt = millis();
            Serial.println("Attempting WiFi reconnection...");
            WiFi.reconnect();
        }
    }

    delay(10);
}
