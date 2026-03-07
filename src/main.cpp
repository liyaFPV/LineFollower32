#include <Arduino.h>
#include "config.h"
#include <WiFi.h>
#include "web.h"

/* ===== PID VARIABLES ===== */
float Kp = 0.0;
float Ki = 0.0;
float Kd = 0.0;

int sensitivity = 0;
int sensorAverage = 0;

bool robotStarted = false;

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

void setup() {
    Serial.begin(115200);

    connectWiFi();

    beginWebServer();
}

void loop() {
  handleWebServer();
  Serial.println(Kp);
  Serial.println(Ki);
  Serial.println(Kd);
  Serial.println(sensitivity);
  sensorAverage=100;
  Serial.println();
}