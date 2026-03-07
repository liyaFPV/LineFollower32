#pragma once

#include <Arduino.h>
#include <Preferences.h>

/*
================================================================================
NETWORK CONFIGURATION
================================================================================
*/
#define SSID "Keenetic"
#define PASSWORD "VPN2011VPN"
#define AP_SSID "LineFollower32"
#define AP_PASS "VPN2011VPN"

/*
================================================================================
PERFORMANCE CONFIGURATION
================================================================================
*/
/* PID control loop frequency: 1000 Hz (1ms interval) */
const int PID_LOOP_FREQUENCY_HZ = 1000;
const int PID_LOOP_INTERVAL_MS = 1000 / PID_LOOP_FREQUENCY_HZ;

/* Telemetry update frequency: 50 Hz (20ms interval) */
const int TELEMETRY_FREQUENCY_HZ = 50;
const int TELEMETRY_INTERVAL_MS = 1000 / TELEMETRY_FREQUENCY_HZ;

/* Web interface refresh frequency: 30 Hz (33ms interval) */
const int WEB_UPDATE_FREQUENCY_HZ = 30;
const int WEB_UPDATE_INTERVAL_MS = 1000 / WEB_UPDATE_FREQUENCY_HZ;

/*
================================================================================
GLOBAL VARIABLES - PID PARAMETERS
================================================================================
*/

/* 
Variable: g_Kp
Purpose: Proportional gain coefficient for PID controller
Range: 0.0 - 10.0
Default: 1.5
Thread safety: Read/write from main loop, web server (volatile access)
*/
extern volatile float g_Kp;

/* 
Variable: g_Ki
Purpose: Integral gain coefficient for PID controller
Range: 0.0 - 1.0
Default: 0.1
Thread safety: Read/write from main loop, web server (volatile access)
*/
extern volatile float g_Ki;

/* 
Variable: g_Kd
Purpose: Derivative gain coefficient for PID controller
Range: 0.0 - 2.0
Default: 0.5
Thread safety: Read/write from main loop, web server (volatile access)
*/
extern volatile float g_Kd;

/*
================================================================================
GLOBAL VARIABLES - CONTROL VARIABLES
================================================================================
*/

/* 
Variable: g_sensitivity
Purpose: Robot sensor sensitivity adjustment (0-100%)
Range: 0 - 100
Default: 50
Usage: Multiplier for error calculation
*/
extern volatile int g_sensitivity;

/* 
Variable: g_sensor_average
Purpose: Current average sensor line position value
Range: 0 - 4095 (12-bit ADC)
Default: 2048 (center)
Updated by: External sensor reading code
Thread safety: Read-only in PID loop, updated externally
*/
extern volatile int g_sensor_average;

/* 
Variable: g_error
Purpose: Current PID error value (difference from center line)
Range: -2048 to +2048
Default: 0
Updated by: PID controller or external code
Thread safety: Read from web server, updated in PID loop
*/
extern volatile int g_error;

/* 
Variable: g_pid_output
Purpose: PID correction output value
Range: -255 to +255
Default: 0
Comment: Result of PID calculation, sent to motor control
Thread safety: Read from web server, updated in PID loop
*/
extern volatile int g_pid_output;

/* 
Variable: g_motor_left
Purpose: Left motor speed/PWM value
Range: -255 to +255 (speed and direction)
Default: 0
Comment: 0 = stopped, positive = forward, negative = backward
Updated by: External motor control code
Thread safety: Read from web server, written by user
*/
extern volatile int g_motor_left;

/* 
Variable: g_motor_right
Purpose: Right motor speed/PWM value
Range: -255 to +255 (speed and direction)
Default: 0
Comment: 0 = stopped, positive = forward, negative = backward
Updated by: External motor control code
Thread safety: Read from web server, written by user
*/
extern volatile int g_motor_right;

/* 
Variable: g_robot_enabled
Purpose: Global robot enable/disable flag
Default: false
Comment: When false, motor outputs should be zeroed
Thread safety: Read from PID loop, written from web server
*/
extern volatile bool g_robot_enabled;

/* 
Variable: g_loop_counter
Purpose: Debug counter for PID loop iterations
Default: 0
Thread safety: Non-critical debug counter
*/
extern volatile unsigned long g_loop_counter;

/*
================================================================================
GLOBAL VARIABLES - TELEMETRY BUFFERS
================================================================================
*/

/* Circular buffer for telemetry data points (max 100 samples) */
const int TELEMETRY_BUFFER_SIZE = 100;

/*
Array: g_telemetry_error
Purpose: Circular buffer of error values for charting
Index: Updated by telemetry task, read by web server
*/
extern volatile int g_telemetry_error[TELEMETRY_BUFFER_SIZE];

/*
Array: g_telemetry_pid_output
Purpose: Circular buffer of PID output values for charting
Index: Updated by telemetry task, read by web server
*/
extern volatile int g_telemetry_pid_output[TELEMETRY_BUFFER_SIZE];

/*
Array: g_telemetry_motor_left
Purpose: Circular buffer of left motor values for charting
Index: Updated by telemetry task, read by web server
*/
extern volatile int g_telemetry_motor_left[TELEMETRY_BUFFER_SIZE];

/*
Array: g_telemetry_motor_right
Purpose: Circular buffer of right motor values for charting
Index: Updated by telemetry task, read by web server
*/
extern volatile int g_telemetry_motor_right[TELEMETRY_BUFFER_SIZE];

/*
Variable: g_telemetry_index
Purpose: Current write index in circular telemetry buffers
Range: 0 to TELEMETRY_BUFFER_SIZE-1
Thread safety: Atomic index for telemetry updates
*/
extern volatile int g_telemetry_index;

/*
================================================================================
SETTINGS MANAGEMENT
================================================================================
*/

extern Preferences preferences;

/*
Function: initializePreferences()
Purpose: Initialize NVS preferences storage for persistent settings
Input: none
Output: true if successful, false otherwise
Notes: Call once in setup()
*/
bool initializePreferences();

/*
Function: saveSettings()
Purpose: Save current PID and sensitivity parameters to NVS flash
Input: none
Output: void
Saved parameters: Kp, Ki, Kd, sensitivity
*/
void saveSettings();

/*
Function: loadSettings()
Purpose: Load PID and sensitivity parameters from NVS flash
Input: none
Output: void
Restores: Kp, Ki, Kd, sensitivity to saved values (or defaults if not found)
*/
void loadSettings();