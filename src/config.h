#pragma once
#include <Arduino.h>
#include <EEPROM.h>

<<<<<<< HEAD
/*
config:
=======
//config
/*
>>>>>>> 50d4e9bedfa809f93c16cf1e3747c99fbb3a9d4a
v1 stable
P=10.00
I=0.00
D=10.00
BaseSpeed=130
TurboSpeed=130
<<<<<<< HEAD
ReturnSpeed=200
=======
>>>>>>> 50d4e9bedfa809f93c16cf1e3747c99fbb3a9d4a
trim=0
timeslep=150
SensorThreshold=4000
*/


/* ===== EEPROM ===== */
#define EEPROM_SIZE 512

/* ===== SENSOR PINS ===== */

#define S0 36
#define S1 39
#define S2 34
#define S3 35
#define S4 32
#define S5 33
#define S6 25
#define S7 26

/* ===== MOTOR PINS ===== */

#define motorL 13
#define motorR 12

/* ===== BUTTON ===== */

#define BTN_START 15

/* ===== PWM ===== */

#define PWM_FREQ 20000
#define PWM_RES 8
#define PWM_CH_L 0
#define PWM_CH_R 1

/* ===== SENSOR COUNT ===== */

#define SENSOR_COUNT 8