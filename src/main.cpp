#include <Arduino.h>
#include "config.h"
#include <WiFi.h>
#include "web.h"
#include "core1.h"
#include <freertos/task.h>
#include <uPID.h>

uPID pid(D_INPUT | I_SATURATE);

float Kp = 0.0;
float Ki = 0.0;
float Kd = 0.0;
int sensitivity = 0;
int sensorAverage = 0;
int sensitivityOffset = 30;
bool robotStarted = false;

int LMS,RMS,Speed=0;

int getE(){
    int sensorV[8]={analogRead(s0),analogRead(s1),analogRead(s2),analogRead(s3),analogRead(s4),analogRead(s5),analogRead(s6),analogRead(s7)};
    for(int i=0;i<8;i++){
        if(sensorV[i]>sensorAverage+sensitivityOffset){
            return SE[i];
        }
    }
    return -1;
}

void connectWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting WiFi");

    int timeout = 20;

    while (WiFi.status() != WL_CONNECTED && timeout--) {
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("\nWiFi fail → AP Mode");

        WiFi.mode(WIFI_AP);
        WiFi.softAP(AP_SSID, AP_PASS);

        Serial.print("AP IP: ");
        Serial.println(WiFi.softAPIP());
    } else {
        Serial.println("\nWiFi connected");
        Serial.println(WiFi.localIP());
    }
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

void selfTest() {
    analogWrite(motorL, 128);
    analogWrite(motorR, 128);
    delay(500);
    analogWrite(motorL, 0);
    analogWrite(motorR, 0);

    int x = analogRead(s0) + analogRead(s1) + analogRead(s2) + analogRead(s3) + analogRead(s4) + analogRead(s5) + analogRead(s6) + analogRead(s7);
    x = x / 8;
    sensorAverage = x;

    Serial.println(Kp);
    Serial.println(Ki);
    Serial.println(Kd);
    Serial.println(sensitivity);

    if (x == 0) {
        Serial.println("Self test Failed");
    } else {
        Serial.println("Self test Passed");
        return;
    }
}

void setup() {
    Serial.begin(9600);
    delay(500);
    pid.setDt(updateTime);
    pid.outMax = 255;
    pid.outMin = -255;
    
    Serial.println("\n\n=== LineFollower32 Starting ===");
    Serial.print("Setup running on core: ");
    Serial.println(xPortGetCoreID());
    
    connectWiFi();
    beginWebServer();
    pinSetup();
    //selfTest();
    analogWrite(motorL, 0);
    analogWrite(motorR, 0);
    initCore1Task();
}

void loop() {
    pid.setKp(10);
    pid.setKi(20);
    pid.setKd(5);
    if (robotStarted) {
        if(digitalRead(butnStart) == LOW) {
            robotStarted = false;
        }
        int E = getE();
        LMS = speed[E][0];
        RMS = speed[E][1];
        analogWrite(motorL, LMS);
        analogWrite(motorR, RMS);
        delay(updateTime);
    }
}