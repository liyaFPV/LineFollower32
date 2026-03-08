#include "core1.h"
#include "web.h"
#include "config.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

extern bool robotStarted;
extern int sensitivity;
extern int sensorAverage;
extern float Kp;
extern float Ki;
extern float Kd;

void readSensorsAndButton() {
    static unsigned long lastButtonPress = 0;
    if(digitalRead(butnStart) == LOW) {
        if (millis() - lastButtonPress > 500) {
            robotStarted = !robotStarted;
            lastButtonPress = millis();
        }
    }
    int x = analogRead(s0) + analogRead(s1) + analogRead(s2) + analogRead(s3) + 
            analogRead(s4) + analogRead(s5) + analogRead(s6) + analogRead(s7);
    sensorAverage = x / 8;
}

void debugPrintState() {
    static unsigned long lastDebug = 0;
    
    if (millis() - lastDebug > 500) {
        lastDebug = millis();
        Serial.println("\n=== LineFollower State ===");
        Serial.print("Robot: ");
        Serial.println(robotStarted ? "RUNNING" : "STOPPED");
        Serial.print("Sensitivity: ");
        Serial.println(sensitivity);
        Serial.print("Sensor Average: ");
        Serial.println(sensorAverage);
        Serial.print("Kp: ");
        Serial.print(Kp, 3);
        Serial.print(" | Ki: ");
        Serial.print(Ki, 3);
        Serial.print(" | Kd: ");
        Serial.println(Kd, 3);
    }
}

void webAndDebugTask(void *parameter) {
    Serial.println("Core 1 (PRO_CPU) task started - Web Server, Debug, Sensors & Button");
    
    while (1) {
        readSensorsAndButton();
        handleWebServer();
        debugPrintState();
        
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
void initCore1Task() {
    xTaskCreatePinnedToCore(
        webAndDebugTask,        /* Функция задачи */
        "WebAndDebugTask",      /* Имя задачи */
        8192,                   /* Размер стека (8KB) */
        NULL,                   /* Параметры */
        1,                      /* Приоритет */
        NULL,                   /* Обработчик задачи */
        PRO_CPU_NUM             /* Ядро 1 (PRO_CPU) */
    );
}
