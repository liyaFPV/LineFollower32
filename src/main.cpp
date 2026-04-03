#include <Arduino.h>
#include "config.h"
#include "web.h"
#include <uPID.h>

uPID pid(D_INPUT | I_SATURATE);
int Err=0;

void getErr(){
    if(analogRead(S0)>=Sk){Err=-3;}
    if(analogRead(S1)>=Sk){Err=-2;}
    if(analogRead(S2)>=Sk){Err=-1;}
    if(analogRead(S3)>=Sk){Err=0;}
    if(analogRead(S4)>=Sk){Err=0;}
    if(analogRead(S5)>=Sk){Err=1;}
    if(analogRead(S6)>=Sk){Err=2;}
    if(analogRead(S7)>=Sk){Err=3;}
}

void setup() {
    Serial.begin(9600);

    pid.setDt(1);
    pid.outMax = 255;
    pid.outMin = -255;

    pinMode(butnStart, INPUT_PULLUP);
    pinMode(S0, INPUT);
    pinMode(S1, INPUT);
    pinMode(S2, INPUT);
    pinMode(S3, INPUT);
    pinMode(S4, INPUT);
    pinMode(S5, INPUT);
    pinMode(S6, INPUT);
    pinMode(S7, INPUT);

    pinMode(motorL, OUTPUT);
    pinMode(motorR, OUTPUT);
    
    setupWEB();
}

void loop() {
    webTick();
    pid.setKp(Pk);
    pid.setKi(Ik);
    pid.setKd(Dk);
    if(digitalRead(butnStart) == LOW) {
        swit = !swit;
        delay(300);
    }
    if (swit){
        getErr();
        float result = pid.compute(Err);
        
        int speedL = constrain(BaseSpeed + result, 0, 255);
        int speedR = constrain(BaseSpeed - result, 0, 255);
        Serial.println("Err: " + String(Err) + " Result: " + String(result) + " SpeedL: " + String(speedL) + " SpeedR: " + String(speedR));
        analogWrite(motorL, speedL);
        analogWrite(motorR, speedR);
    }else{
        analogWrite(motorL, 0);
        analogWrite(motorR, 0);
    }
}