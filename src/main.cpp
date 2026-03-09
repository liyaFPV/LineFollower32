#include <Arduino.h>
#include "config.h"
bool robotStarted = false;
int LMS,RMS,Speed=0;


void pinSetup() {
    pinMode(s0, INPUT);
    pinMode(s1, INPUT);
    pinMode(s2, INPUT);
    pinMode(s3, INPUT);
    pinMode(s4, INPUT);
    pinMode(s5, INPUT);
    pinMode(s6, INPUT);
    pinMode(s7, INPUT);
    pinMode(butnStart, INPUT_PULLUP);

    pinMode(motorL, OUTPUT);
    pinMode(motorR, OUTPUT);
}

void setup() {
    Serial.begin(9600);
    pinSetup();
    analogWrite(motorL, 0);
    analogWrite(motorR, 0);
}

void race(){
    if(analogRead(s0)>=sensitivity){
        Speed=255;
        LMS=constrain(BaseSpeed+Speed,0,255);
        RMS=constrain(BaseSpeed-Speed,0,255);
    }
    if(analogRead(s1)>=sensitivity){
        Speed=200;
        LMS=constrain(BaseSpeed+Speed,0,255);
        RMS=constrain(BaseSpeed-Speed,0,255);
    }
    if(analogRead(s2)>=sensitivity){
        Speed=127;
        LMS=constrain(BaseSpeed+Speed,0,255);
        RMS=constrain(BaseSpeed-Speed,0,255);
    }
    if(analogRead(s3)>=sensitivity){
        Speed=0;
        LMS=constrain(BaseSpeed+Speed,0,255);
        RMS=constrain(BaseSpeed-Speed,0,255);
    }
    if(analogRead(s4)>=sensitivity){
        Speed=0;
        LMS=constrain(BaseSpeed+Speed,0,255);
        RMS=constrain(BaseSpeed-Speed,0,255);
    }
    if(analogRead(s5)>=sensitivity){
        Speed=127;
        LMS=constrain(BaseSpeed+Speed,0,255);
        RMS=constrain(BaseSpeed-Speed,0,255);
    }
    if(analogRead(s6)>=sensitivity){
        Speed=200;
        LMS=constrain(BaseSpeed+Speed,0,255);
        RMS=constrain(BaseSpeed-Speed,0,255);
    }
    if(analogRead(s7)>=sensitivity){
        Speed=255;
        LMS=constrain(BaseSpeed+Speed,0,255);
        RMS=constrain(BaseSpeed-Speed,0,255);
    }
}


void loop() {
    if (robotStarted) {
        
    }
}