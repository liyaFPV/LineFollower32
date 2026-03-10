#include <Arduino.h>
#include "config.h"
bool robotStarted = false;
int LMS,RMS,Speed=0;

void ATcontrol(){
    if (Serial.available()) {
        int spd = Serial.parseInt();
        spd = constrain(spd, 0, 255);

        LMS = spd;
        RMS = spd;
    }   
}

int getE(){
    int sensorV[8]={analogRead(s0),analogRead(s1),analogRead(s2),analogRead(s3),analogRead(s4),analogRead(s5),analogRead(s6),analogRead(s7)};
    for(int i=0;i<8;i++){
        if(sensorV[i]>=sensitivity){
            return SE[i];
        }
    }
    return -1;
}

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
        Speed=50;
        LMS=constrain(BaseSpeed+Speed,0,255);
        RMS=constrain(BaseSpeed-Speed,0,255);
    }
    if(analogRead(s1)>=sensitivity){
        Speed=40;
        LMS=constrain(BaseSpeed+Speed,0,255);
        RMS=constrain(BaseSpeed-Speed,0,255);
    }
    if(analogRead(s2)>=sensitivity){
        Speed=30;
        LMS=constrain(BaseSpeed+Speed,0,255);
        RMS=constrain(BaseSpeed-Speed,0,255);
    }
    if(analogRead(s3)>=sensitivity){
        Speed=0;
        LMS=constrain(BaseSpeed-Speed,0,255);
        RMS=constrain(BaseSpeed+Speed,0,255);
    }
    if(analogRead(s4)>=sensitivity){
        Speed=0;
        LMS=constrain(BaseSpeed-Speed,0,255);
        RMS=constrain(BaseSpeed+Speed,0,255);
    }
    if(analogRead(s5)>=sensitivity){
        Speed=30;
        LMS=constrain(BaseSpeed-Speed,0,255);
        RMS=constrain(BaseSpeed+Speed,0,255);
    }
    if(analogRead(s6)>=sensitivity){
        Speed=40;
        LMS=constrain(BaseSpeed-Speed,0,255);
        RMS=constrain(BaseSpeed+Speed,0,255);
    }
    if(analogRead(s7)>=sensitivity){
        Speed=50;
        LMS=constrain(BaseSpeed-Speed,0,255);
        RMS=constrain(BaseSpeed+Speed,0,255);
    }
}


void loop() {
    if(digitalRead(butnStart) == LOW) {
        robotStarted = !robotStarted;
        LMS=0;
        RMS=0;
        delay(500);
    }
    if (robotStarted) {
        race();
        analogWrite(motorL, LMS+add);
        analogWrite(motorR, RMS+add);
    }else{
        ATcontrol();
        analogWrite(motorL, LMS);
        analogWrite(motorR, RMS);
    }
    int x = analogRead(s0);
    Serial.println("E:"+String(getE())+","+"LMS:"+String(LMS)+","+"RMS:"+String(RMS)+","+"x:"+String(x)+","+"start:"+String(robotStarted));

    //delay(10);
}