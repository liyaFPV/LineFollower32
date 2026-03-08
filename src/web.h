#pragma once

#include <WebServer.h>
#include <Preferences.h>

/* External robot variables */

extern float Kp;
extern float Ki;
extern float Kd;

extern int sensitivity;
extern int sensorAverage;
extern int sensitivityOffset;

extern bool robotStarted;

void beginWebServer();
void handleWebServer();