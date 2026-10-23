
#include <Arduino.h>

void shortBlink() {
    digitalWrite(LED_BUILTIN, LOW);
    delay(400);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(400);
}

void longBlink() {
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(800);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
}

void pauseSymbol() {
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT); //set as output pin
    digitalWrite(LED_BUILTIN, LOW); //initialize with low
}

void loop() {
    shortBlink();
    longBlink();
    pauseSymbol();

    longBlink();
    shortBlink();
    shortBlink();
    pauseSymbol();

    longBlink();
    shortBlink();
    pauseSymbol();
}
