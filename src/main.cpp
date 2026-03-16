#include <Arduino.h>
#include "config.h"
#include "web.h"
void setup() {
    Serial.begin(9600);
    pinMode(butnStart, INPUT_PULLUP);
    setupWEB();
}

void loop() {
    webTick();
    if(digitalRead(butnStart) == LOW) {
        swit = !swit;
        delay(300);
    }
}