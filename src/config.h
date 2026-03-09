#pragma once

#include <Arduino.h>

/* ===== WIFI CONFIG ===== */
#define WIFI_SSID "Keenetic"
#define WIFI_PASSWORD "VPN2011VPN"

#define AP_SSID "LineFollower32"
#define AP_PASS "VPN2011VPN"

/* ===== SENSOR PINOUT ===== */
#define s0 36
#define s1 39
#define s2 34
#define s3 35
#define s4 32
#define s5 33
#define s6 25
#define s7 26

/* ===== MOTOR PINS ===== */
#define motorL 12
#define motorR 13

/*=====BUTTON PINS=====*/
#define butnStart 0

/*=====PID PARAMETERS=====*/
#define BaseSpeed 100
#define MaxSpeed 200
#define updateTime 1
const int SE[] = {0,1,2,3,4,5,6,7};
const int speed[][2] = {
    {0,255},//lms rms
    {150,255},
    {150,255},
    {255,255},
    {255,255},
    {255,150},
    {255,100},
    {255,0}
};