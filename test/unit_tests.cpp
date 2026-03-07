#ifdefined UNIT_TESTS

#include <unity.h>
#include <Arduino.h>
#include "config.h"
#include "motors.h"
#include "sensors.h"
#include "safety.h"

/*
================================================================================
UNIT TESTS - Motor Control Module
================================================================================
*/

void test_motor_speed_clamping() {
    /* Test that motor speeds are clamped to valid range */
    motorSetSpeed(500, 500);  /* Over limit */
    TEST_ASSERT_EQUAL_INT8(motor_speed_left, 127);
    TEST_ASSERT_EQUAL_INT8(motor_speed_right, 127);
    
    motorSetSpeed(-500, -500);  /* Under limit */
    TEST_ASSERT_EQUAL_INT8(motor_speed_left, -127);
    TEST_ASSERT_EQUAL_INT8(motor_speed_right, -127);
}

void test_motor_stop() {
    motorSetSpeed(100, 100);
    motorStop();
    TEST_ASSERT_EQUAL_INT8(motor_speed_left, 0);
    TEST_ASSERT_EQUAL_INT8(motor_speed_right, 0);
}

void test_motor_direction() {
    /* Forward */
    motorSetSpeed(100, 100);
    TEST_ASSERT_EQUAL_INT8(motor_speed_left, 100);
    
    /* Backward */
    motorSetSpeed(-100, -100);
    TEST_ASSERT_EQUAL_INT8(motor_speed_left, -100);
}

/*
================================================================================
UNIT TESTS - Sensor Module
================================================================================
*/

void test_sensor_read_range() {
    for (int i = 0; i < 8; i++) {
        TEST_ASSERT_LESS_OR_EQUAL(4095, sensor_raw[i]);
        TEST_ASSERT_GREATER_OR_EQUAL(0, sensor_raw[i]);
    }
}

void test_sensor_average_calculation() {
    /* Set all sensors to same value */
    for (int i = 0; i < 8; i++) {
        sensor_raw[i] = 2000;
    }
    sensorCalculatePosition();
    TEST_ASSERT_EQUAL_INT(sensor_average, 2000);
}

/*
================================================================================
UNIT TESTS - PID Controller
================================================================================
*/

void test_pid_bounds() {
    /* Ensure PID output stays within bounds */
    TEST_ASSERT_LESS_OR_EQUAL(255, g_pid_output);
    TEST_ASSERT_GREATER_OR_EQUAL(-255, g_pid_output);
}

void test_pid_zero_error() {
    g_error = 0;
    executePIDLoop();
    /* At zero error, PID should tend toward zero */
    TEST_ASSERT_LESS_OR_EQUAL(50, abs(g_pid_output));
}

/*
================================================================================
UNIT TESTS - Config & Safety
================================================================================
*/

void test_watchdog_initialized() {
    TEST_ASSERT_TRUE(safety_status.watchdog_ok);
}

void test_preferences_load() {
    TEST_ASSERT_GREATER_THAN_FLOAT(0.0f, g_Kp);
    TEST_ASSERT_GREATER_THAN_FLOAT(0.0f, g_Ki);
    TEST_ASSERT_GREATER_THAN_FLOAT(0.0f, g_Kd);
}

/*
================================================================================
Test Suite Runner
================================================================================
*/

void setUp() {
    /* Setup code before each test */
}

void tearDown() {
    /* Cleanup after each test */
}

void run_unit_tests() {
    UNITY_BEGIN();
    
    /* Motor tests */
    RUN_TEST(test_motor_speed_clamping);
    RUN_TEST(test_motor_stop);
    RUN_TEST(test_motor_direction);
    
    /* Sensor tests */
    RUN_TEST(test_sensor_read_range);
    RUN_TEST(test_sensor_average_calculation);
    
    /* PID tests */
    RUN_TEST(test_pid_bounds);
    
    /* Config tests */
    RUN_TEST(test_watchdog_initialized);
    RUN_TEST(test_preferences_load);
    
    UNITY_END();
}

#endif
