#pragma once
#include <Arduino.h>
#include <EEPROM.h>

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

#define motorL 12
#define motorR 13

/* ===== BUTTON ===== */

#define BTN_START 15

/* ===== PWM ===== */

#define PWM_FREQ 20000
#define PWM_RES 8
#define PWM_CH_L 0
#define PWM_CH_R 1

/* ===== SENSOR COUNT ===== */

#define SENSOR_COUNT 8